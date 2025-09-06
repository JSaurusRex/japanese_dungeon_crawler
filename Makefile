main:
	gcc -o JapaneseLearningGame src/*.c src/enemies/*.c src/items/*.c src/items/attacks/*.c src/shields/*.c src/generation/*.c -lraylib -lm -ggdb