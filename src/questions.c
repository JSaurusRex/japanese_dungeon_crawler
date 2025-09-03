#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
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
int _level = 1;

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

    _feedback_alpha = 0;
    _screen = &QuestionsFrame;
    _answers_amount = amount;
    _answer_counter = 0;
    _answers_incorrect = 0;
    _quiz_hearts = hearts;
    strncpy(_pack, pack, STRING_LENGTH);

    _callback = callback;

    strncpy(_pack, pack, STRING_LENGTH);
    _level = level;

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

        if (IsKeyPressed(KEY_ENTER))
        {
            _answer_counter++;
            _feedback_alpha = 1;

            if (strncmp(_question.english, _input_str, STRING_LENGTH) == 0)
            {
                //correct
                FeedbackColor = GREEN;

                if (_answer_counter >= _answers_amount)
                {
                    (*_callback)();
                    return;
                }
            }
            else
            {
                FeedbackColor = RED;
                _answers_incorrect++;

                if (_answers_incorrect >= _quiz_hearts)
                {
                    (*_callback)();
                    return;
                }
            }

            LoadRandomQuestion(_pack, &_question, _level);
            while(_input_str_cursor >= 0)
            {
                _input_str[_input_str_cursor] = 0;
                _input_str_cursor--;
            }
        }
    }

    //rendering
    {
        ClearBackground(ColorLerp(RAYWHITE, FeedbackColor, powf(_feedback_alpha, 2.0)));
        
        _feedback_alpha -= GetFrameTime();
        if (_feedback_alpha < 0)
            _feedback_alpha = 0;
        
        //counter
        {
            char str[16];
            snprintf(str, 16, "%i/%i", _answer_counter, _answers_amount);
            DrawTextEx(_fontJapanese, str, (Vector2){ 50, 50 }, 30, 2, BLACK);
        }

        //hearts
        for(int i = 0; i < _quiz_hearts-_answers_incorrect; i++)
        {
            DrawRectangle(50 + i * 50, 90, 30, 30, RED);
        }

        //question
        DrawTextEx(_fontJapanese, _question.japanese, (Vector2){ 400, 100 }, 50, 2, BLACK);

        //input
        DrawTextEx(_fontJapanese, _input_str, (Vector2){ 400, 200 }, 50, 2, BLACK);

        //render all answers
        int shown = 0;
        for(int i = 0; i < _questions_count && shown < 11; i++)
        {
            if(_input_str_cursor == -1 || strstr(_all_questions[i].english, _input_str))
            {
                DrawTextEx(_fontJapanese, _all_questions[i].english, (Vector2){ 400, 250 + shown * 40 }, 35, 2, BLACK);
                shown++;
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

    char file[STRING_LENGTH];
    snprintf(file, STRING_LENGTH, "data/words/%s/levels.txt", pack);
    char * text = LoadFileText(file);
    if (!text)
    {
        printf("error: LoadRandomQuestion: LoadFileText failed to load file %s\n", file);
        return false;
    }
    int levels = atoi(text);

    level = rand() % (int)fmax(fmin(levels, level)+1, 1);
    printf("random level %i, max %i, levels.txt: %s\n", level, levels, text);

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

    if(_printDebug)
    {
        printf("debug: LoadRandomQuestion: new english word: %s\n", question->english);
        printf("debug: LoadRandomQuestion: new japanese word: %s\n", question->japanese);
    }

    return true;
}

bool ProcessQuestionsFile(char * file, sQuestion * question)
{
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