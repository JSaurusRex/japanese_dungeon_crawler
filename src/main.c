#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <raylib.h>
#include <math.h>

#define LATIN_UNICODE 128
#define STRING_LENGTH 256

bool _printDebug = true;


typedef struct
{
    char japanese[STRING_LENGTH];
    char english[STRING_LENGTH];
} sQuestion;

sQuestion _question;

sQuestion * _all_questions = 0;
int _questions_count = 0;

char _input_str[STRING_LENGTH] = "";
int _input_str_cursor = -1;
float _feedback_alpha = 1;


bool LoadRandomQuestion(char * pack, sQuestion * question);
bool LoadAllQuestions();
bool ProcessQuestionsFile(char * file, sQuestion * question);

Color FeedbackColor = GREEN;

void main() {
    InitWindow(800, 600, "Japanese text test");

    int total = (0x9FFF - 0x3040) + LATIN_UNICODE;

    // Allocate and fill codepoint list
    int *codepoints = malloc(total * sizeof(int));
    for(int i = 0; i < total; i++)
    {
        codepoints[i] = 0x3040 + (i - LATIN_UNICODE);
        if ( i < LATIN_UNICODE )
            codepoints[i] = i;
    }

    // Load font with all Japanese glyphs
    Font fontJapanese = LoadFontEx("data/fonts/NotoSansJP-Regular.ttf", 50, codepoints, total);

    free(codepoints);

    LoadAllQuestions();
    LoadRandomQuestion("hiragana", &_question);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

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
                _feedback_alpha = 1;

                if (strncmp(_question.english, _input_str, STRING_LENGTH) == 0)
                {
                    //correct
                    FeedbackColor = GREEN;
                }
                else
                {
                    FeedbackColor = RED;
                }

                LoadRandomQuestion("hiragana", &_question);
                while(_input_str_cursor >= 0)
                {
                    _input_str[_input_str_cursor] = 0;
                    _input_str_cursor--;
                }
            }
        }

        //rendering
        {
            BeginDrawing();


            ClearBackground(ColorLerp(RAYWHITE, FeedbackColor, powf(_feedback_alpha, 2.0)));
            
            _feedback_alpha -= GetFrameTime();
            if (_feedback_alpha < 0)
                _feedback_alpha = 0;

            DrawTextEx(fontJapanese, _question.japanese, (Vector2){ 400, 100 }, 50, 2, BLACK);

            DrawTextEx(fontJapanese, _input_str, (Vector2){ 400, 200 }, 50, 2, BLACK);

            //render all answers
            int shown = 0;
            for(int i = 0; i < _questions_count && shown < 5; i++)
            {
                if(_input_str_cursor == -1 || strstr(_all_questions[i].english, _input_str))
                {
                    DrawTextEx(fontJapanese, _all_questions[i].english, (Vector2){ 400, 300 + shown * 50 }, 35, 2, BLACK);
                    shown++;
                }
            }

            EndDrawing();
        }
    }

    UnloadFont(fontJapanese);
    CloseWindow();
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
    
    _all_questions = calloc(sizeof(char), STRING_LENGTH * files.count * sizeof(char));

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

    _questions_count = index;
}

bool LoadRandomQuestion(char * pack, sQuestion * question)
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