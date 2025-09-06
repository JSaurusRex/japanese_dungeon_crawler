#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include "rendering.h"
#include <raymath.h>
#include <math.h>

#include "main.h"
#include "questions.h"

sQuestion _question;

sQuestion * _all_questions = 0;
int _questions_count = 0;

char _input_str[STRING_LENGTH] = "";
int _input_str_cursor = -1;
float _feedback_alpha = 1;

int _answer_counter = 0;
int _answers_amount = 5;
int _quiz_hearts = 3;
int _answers_incorrect = 3;
char _pack [STRING_LENGTH] = {0};
int _pack_level = 1;
int _combo = 0;

int _question_effectiveness = 0;

bool _show_answer = false;

typedef enum {ENGLISH, ROMAJI} QuestionType;

QuestionType _questionType;

void (*_callback)() = 0;


Color FeedbackColor = GREEN;

bool Questions_init()
{
    if (!LoadAllQuestions())
    {
        printf("error: Questions_init: LoadAllQuestions() failed\n");
        return false;
    }

    return true;
}

void Start_Questions(int amount, int hearts, char * pack, int level, void (*callback)())
{
    if (!pack)
    {
        printf("error: Start_Questions: argument pack is null\n");
        return;
    }

    char path[STRING_LENGTH];
    snprintf(path, STRING_LENGTH, "data/words/%s", pack);

    if (!DirectoryExists(path))
    {
        printf("error: Start_Questions: Directory does not exist %s\n", path);
        return;
    }

    _combo = 0;

    _feedback_alpha = 0;
    change_screen(&QuestionsFrame);
    _answers_amount = amount;
    _answer_counter = 0;
    _answers_incorrect = 0;
    _quiz_hearts = hearts;
    _show_answer = false;
    _question_effectiveness = 0;
    strncpy(_pack, pack, STRING_LENGTH);

    _callback = callback;

    strncpy(_pack, pack, STRING_LENGTH);
    _pack_level = level;

    LoadRandomQuestion(pack, &_question, level);
    
    while(_input_str_cursor >= 0)
    {
        _input_str[_input_str_cursor] = 0;
        _input_str_cursor--;
    }
}

void QuestionsFrame()
{
    //input
    {
        if (!_show_answer)
        {
            char c = GetCharPressed();

            while(c && _input_str_cursor < STRING_LENGTH)
            {
                if (_printDebug)
                    printf("debug: user pressed %c, cursor %i\n", c, _input_str_cursor);
                
                _input_str_cursor++;
                _input_str[_input_str_cursor] = c;
                c = GetCharPressed();
            }

            if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE))
            {
                if (_printDebug)
                    printf("debug: user pressed backspace\n");
                
                while(_input_str_cursor >= 0)
                {
                    _input_str[_input_str_cursor] = 0;
                    _input_str_cursor--;

                    if (!IsKeyDown(KEY_LEFT_CONTROL))
                        break;
                }
            }
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            if (!_show_answer)
            {
                _answer_counter++;
                _feedback_alpha = 1;
                _show_answer = true;

                char * answer = _question.english;

                if (_questionType == ROMAJI)
                    answer = _question.romaji;

                if (strncmp(answer, _input_str, STRING_LENGTH) == 0)
                {
                    //correct
                    FeedbackColor = GREEN;
                    _combo++;
                    _question_effectiveness += _question.level;
                }
                else
                {
                    FeedbackColor = RED;
                    _answers_incorrect++;
                    _combo = 0;                    
                }
            }else
            {
                _show_answer = false;
                while(_input_str_cursor >= 0)
                {
                    _input_str[_input_str_cursor] = 0;
                    _input_str_cursor--;
                }

                if (_answer_counter >= _answers_amount || _answers_incorrect >= _quiz_hearts)
                {
                    (*_callback)();
                    return;
                }else
                {
                    LoadRandomQuestion(_pack, &_question, _pack_level);
                }
            }
        }
    }

    //rendering
    {
        ClearBackground(ColorLerp(RAYWHITE, FeedbackColor, powf(_feedback_alpha, 2.0)));
        
        if (!_show_answer)
        {
            _feedback_alpha -= GetFrameTime();
            if (_feedback_alpha < 0)
                _feedback_alpha = 0;
        }
        
        //counter
        {
            char str[16];
            snprintf(str, 16, "%i/%i", _answer_counter, _answers_amount);
            drawTextEx(_fontJapanese, str, (Vector2){ 50, 50 }, 30, 2, BLACK);
        }

        //combo
        {
            char str[16];
            snprintf(str, 16, "combo %i", _combo);
            drawTextEx(_fontJapanese, str, (Vector2){ 600, 15 }, 50, 2, BLACK);
        }

        //effectiveness
        {
            char str[32];
            snprintf(str, 32, "level %i\nTotal %.1f", _question.level, (float)_question_effectiveness / (float)(_answers_amount));
            drawTextEx(_fontJapanese, str, (Vector2){ 50, 400 }, 40, 2, BLACK);
        }

        //hearts
        for(int i = 0; i < _quiz_hearts-_answers_incorrect; i++)
        {
            drawRectangle(50 + i * 50, 90, 30, 30, RED);
        }

        //question
        drawTextEx(_fontJapanese, _question.japanese, (Vector2){ 400, 100 }, 50, 2, BLACK);

        //input
        drawRectangle(280, 195, 300, 60, ColorAlpha(YELLOW, 0.5));
        drawTextEx(_fontJapanese, _input_str, (Vector2){ 300, 200 }, 50, 2, BLACK);

        //romaji or english

        if (_questionType == ROMAJI)
            drawTextEx(_fontJapanese, "ROMAJI", (Vector2){ 30, 200 }, 50, 2, BLACK);
        else
            drawTextEx(_fontJapanese, "ENGLISH", (Vector2){ 30, 200 }, 50, 2, BLACK);
        
        //draw answer
        if (_show_answer)
        {
            if (_questionType == ROMAJI)
            {
                drawTextEx(_fontJapanese, _question.romaji, (Vector2){ 400, 150 }, 50, 2, BLACK);
                drawTextEx(_fontJapanese, _question.english, (Vector2){ 400, 250 }, 50, 2, BLACK);
            }
            else
            {
                drawTextEx(_fontJapanese, _question.english, (Vector2){ 400, 150 }, 50, 2, BLACK);
                drawTextEx(_fontJapanese, _question.romaji, (Vector2){ 400, 250 }, 50, 2, BLACK);
            }
        }

        if (_answers_incorrect == 0)
            drawTextEx(_fontJapanese, "Perfect!", (Vector2){ 320, 15 }, 50, 2, GOLD);

        //render all answers
        if (_questionType == ENGLISH && !_show_answer)
        {
            int shown = 0;
            for(int i = 0; i < _questions_count && shown < 11; i++)
            {
                if(_input_str_cursor == -1 || strstr(_all_questions[i].english, _input_str))
                {
                    drawTextEx(_fontJapanese, _all_questions[i].english, (Vector2){ 400, 250 + shown * 40 }, 35, 2, BLACK);
                    shown++;
                }
            }
        }
    }
}

bool LoadAllQuestions()
{
    if (!DirectoryExists("data/words"))
    {
        printf("error: LoadAllQuestions: Directory does not exist %s\n", "data/words");
        return false;
    }

    FilePathList files = LoadDirectoryFilesEx("data/words", ".question", true);

    if (files.count <= 0)
    {
        printf("error: LoadAllQuestions: No files in directory %s\n", "data/words");
        return false;
    }

    if (_printDebug)
    {
        printf("debug: LoadAllQuestions: Found %i answers\n", files.count);
    }

    if (_all_questions)
        printf("error: LoadAllQuestions: Answers already allocated\n");
    
    _all_questions = calloc(sizeof(sQuestion), files.count);

    if (!_all_questions)
    {
        printf("error: LoadAllQuestions: Failed to allocate\n");
        return false;
    }

    int index = 0;
    for(int i = 0; i < files.count; i++)
    {
        if (!ProcessQuestionsFile(files.paths[i], &_all_questions[index]))
            continue;
        
        index++;
    }

    printf("index %i\n", index);

    _questions_count = index;
    return true;
}

int GetMaxLevel(char * pack)
{
    if (!pack)
    {
        printf("error: GetMaxLevel: argument pack is null\n");
        return 0;
    }
    
    char path[STRING_LENGTH];
    snprintf(path, STRING_LENGTH, "data/words/%s", pack);

    if (!DirectoryExists(path))
    {
        printf("error: GetMaxLevel: Directory does not exist %s\n", path);
        return 0;
    }

    char file[STRING_LENGTH];
    snprintf(file, STRING_LENGTH, "data/words/%s/levels.txt", pack);
    char * text = LoadFileText(file);
    if (!text)
    {
        printf("error: GetMaxLevel: LoadFileText failed to load file %s\n", file);
        return 0;
    }
    
    int level = atoi(text);

    if (_printDebug)
        printf("debug: GetMaxLevel: level found is %i for pack %s\n", level, pack);

    return level;
}

bool LoadRandomQuestion(char * pack, sQuestion * question, int level)
{
    if (!pack)
    {
        printf("error: LoadRandomQuestion: argument pack is null\n");
        return false;
    }
    
    char path[STRING_LENGTH];
    snprintf(path, STRING_LENGTH, "data/words/%s", pack);

    if (!DirectoryExists(path))
    {
        printf("error: LoadRandomQuestion: Directory does not exist %s\n", path);
        return false;
    }

    int max = GetMaxLevel(pack);

    max = fmin(max, level);

    level = fmin(1 + rand() % (int)fmax(max+2, 1), max);

    printf("max level %i\n", (int)fmax(fmin(max, max), 1));
    printf("chosen level %i\n", level);

    snprintf(path, STRING_LENGTH, "data/words/%s/%i", pack, level);

    FilePathList files = LoadDirectoryFilesEx(path, ".question", false);

    if (files.count <= 0)
    {
        printf("error: LoadRandomQuestion: No files in directory %s\n", path);
        return false;
    }

    if (_printDebug)
    {
        for(int i = 0; i < files.count; i++)
        {
                printf("debug: LoadRandomQuestion: path %i: %s\n", i, files.paths[i]);
        }
    }

    int randomIndex = rand() % files.count;

    ProcessQuestionsFile(files.paths[randomIndex], question);
    question->level = level;

    _questionType = rand() % (ENGLISH + 1);

    if (question->english[0] == 0)
        _questionType = ROMAJI;
    
    if (question->romaji[0] == 0)
        _questionType = ENGLISH;

    if(_printDebug)
    {
        printf("debug: LoadRandomQuestion: new english word: %s\n", question->english);
        printf("debug: LoadRandomQuestion: new japanese word: %s\n", question->japanese);
    }

    return true;
}

bool ProcessQuestionsFile(char * file, sQuestion * question)
{
    sQuestion empty = {0};
    *question = empty;

    char * text = LoadFileText(file);
    if (!text)
    {
        printf("error: ProcessQuestionsFile: LoadFileText failed to load file %s\n", file);
        return false;
    }

    //process file
    int index = 0;
    int length = strlen(text);
    for(int line = 0; line < 5; line++)
    {
        if(index >= length)
            break;
        
        char c = text[index];
        index += 2; //skip :

        if (index >= length)
            break;
        
        // if (_printDebug)
        //     printf("debug: ProcessQuestionsFile: c = %c\n", c);
        
        switch(c)
        {
            case 'j':
            {
                //copy string
                int i = 0;
                while(index < length && text[index] != '\n' && i < STRING_LENGTH-1)
                {
                    question->japanese[i] = text[index];
                    index++;
                    i++;
                }
                
                question->japanese[i] = 0;
                break;
            }
            
            case 'e':
            {
                //copy string
                int i = 0;
                while(index < length && text[index] != '\n' && i < STRING_LENGTH-1)
                {
                    question->english[i] = text[index];
                    index++;
                    i++;
                }
                
                question->english[i] = 0;
                break;
            }

            case 'r':
            {
                //copy string
                int i = 0;
                while(index < length && text[index] != '\n' && i < STRING_LENGTH-1)
                {
                    question->romaji[i] = text[index];
                    index++;
                    i++;
                }
                
                question->romaji[i] = 0;
                break;
            }
        }

        //goto next line
        while(index < length && text[index] != '\n')
        {
            index++;
        }
        index++; //atm its at '\n' so we have to move 1 further
    }

    UnloadFileText(text);
    
    return true;
}