#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "main.h"

typedef struct
{
    char japanese[STRING_LENGTH];
    char english[STRING_LENGTH];
    char romaji[STRING_LENGTH];
    char kana[STRING_LENGTH];
    int level;
} sQuestion;

extern int _quiz_hearts;
extern int _answers_incorrect;
extern int _answers_amount;
extern int _combo;
extern int _question_effectiveness;
extern int _pack_level;

bool Questions_init();
void Start_Questions(int amount, int hearts, char * pack, int level, void (*callback)());

void QuestionsFrame();
int GetMaxLevel(char * pack);

bool LoadRandomQuestion(char * pack, sQuestion * question, int level);
bool LoadAllQuestions();
bool ProcessQuestionsFile(char * file, sQuestion * question);

#endif