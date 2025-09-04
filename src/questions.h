#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "main.h"

typedef struct
{
    char japanese[STRING_LENGTH];
    char english[STRING_LENGTH];
    char romaji[STRING_LENGTH];
} sQuestion;

extern int _quiz_hearts;
extern int _answers_incorrect;
extern int _combo;

bool Questions_init();
void Start_Questions(int amount, int hearts, char * pack, int level, void (*callback)());

void QuestionsFrame();


bool LoadRandomQuestion(char * pack, sQuestion * question, int level);
bool LoadAllQuestions();
bool ProcessQuestionsFile(char * file, sQuestion * question);

#endif