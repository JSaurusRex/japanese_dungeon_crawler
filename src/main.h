#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>

#define LATIN_UNICODE 128
#define STRING_LENGTH 256

extern bool _printDebug;
extern Font _fontJapanese;

// extern void (*_screen)();
void change_screen(void (*next_screen)());



#endif