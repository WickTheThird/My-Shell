#include "./myshell.h"
#include "./iored.c"

//TITLE CALLER --> handles the commands
//> CHECKER 
//>         --> stores the location of the redirection symbol;
//>         --> if there are no redirection symbols, then it returns to the caller and tries to execute the command;
//>         --> given that this acts as a flag, it shall return 1 if no symbols are found, and 0 if they are found;
//>              this is because some values might not have a symbol, but they might have a command, or the other way around;
int checker(char** input) { 
    int noTokens = arrLen(input);

    int* symbolsPos = posSymbols(input, 2);
    int* whichSymbols = posSymbols(input, 1);

    int counter = 0;
    for (; whichSymbols[counter] != ENDOFINTARR; ++counter);

    int num_bg_procs = 0;
    if (checkSymbolPos(symbolsPos, whichSymbols, noTokens) == 0 && symbolsPos[0] != ENDOFINTARR) {
        int count = 0;
        int bg_process = 0;
        for (int i = 0; whichSymbols[i] != ENDOFINTARR; ++i, ++count) {
            if (strcmp(symbols[whichSymbols[counter]].name, "&") == 0) {
                bg_process = 1;
                continue;
            }

            if (bg_process == 1) {
                printf("Background process started.\n");
                num_bg_procs++;
                rBackground(input, symbolsPos[count], symbolsPos, count);
                bg_process = 0;
            } else {
                symbols[whichSymbols[i]].func(input, symbolsPos[count], symbolsPos, count);
            }
        }

        return 1;
    }

    return 0;
}

char** input() {
    // printf("step 1\n");
    printf("%s ", PWD());
    printf("/> ");

    char* in = malloc(INPUT_SIZE * sizeof(char));
    fgets(in, INPUT_SIZE, stdin);

    if (strLen(in) && in[0] == '\n') {
        input();
    }

    char** inp = tokeniser(in);

    return inp;
}

int caller(char** input, int noCommands) {
    // printf("caller\n");
    int flag = 0;

    if (noCommands < 1) {
        printf("No command entered\n");
        return 1;
    } if (strcmp(strip(input[0]), "quit") == 0) {
        return 0;
    }

    flag = checker(input);

    if (flag == 1) {
        return 1;
    }

    commandRunner(input, posSymbols(input, 2));

    return 1;
}

// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.

// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.

// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.

// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study. 

// FILIP BUMBU, 21373616
