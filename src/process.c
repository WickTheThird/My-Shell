#include "./myshell.h"

//* this is a flag that is used in maps or other which require a function call to end or compare to ENDARR
char* ENDARR() {
    return "ENDARR";
}

//TITLE: COMMAND DEPENDECIES
//> DIRCHECKER
//* checks if the input is a directory
int dirCheck(const char* input) {
    struct stat path;
    printf("%s\n", input);
    stat(input, &path);

    return S_ISREG(path.st_mode);
}

//> FILECHECKER
//* checks if the input is a file, and can access it, returning the flag value
int fileCheck(const char* input) {
    return access(input, F_OK);
}

//TITLE: OTHER PROCESS FUNCTIONS
//* string length
//* returns the length of the string, not the size of char bits
int strLen(char* input) {
    int count = 0;
    while (input[count] != '\0') {
        ++count;
    }

    return count;
}

//* array length --> for now it works with NULL as the end of the array
//* counts the tokens in the array
//* this works only with char** arrays
int arrLen(char** input) {
    int count = 0;

    while (input[count] != NULL) {
        ++count;
    }

    return count;
}

//* stores the beggining of the pattern --> for now we want it to work just with withespaces;
int* tokensPos(char* input, int len) {
    int* tokens = malloc((len + 1) * sizeof(int));
    *(tokens) = 0; // stores the beggining of the string (0)

    int foundQ = 0;
    int count = 1;
    int i = 0;
    for (; i < len; ++i) {

        if ((input[i] == '\"' || input[i] == '\'') && foundQ == 0) {
            foundQ = 1;
            *(tokens + count) = i + 1;
        } else if ((input[i] == '\"' || input[i] == '\'') && foundQ == 1) {
            foundQ = 0;
            *(tokens + count) = i - 1;
        }
        
        if (input[i] == ' ' && foundQ == 0) {
            *(tokens + count) = i + 1;
            ++count;
        } 
    }

    //? stores the end of the string
    *(tokens + count) = i;

    return tokens;
}

//* conts how many tokens are in the input
//* one being defied by whitespace
int countTokens(char* input, int len) {
    int noTokens = 0;
    int foundQ = 0;
    for (int i = 0; i < len; ++i) {
        
        if ((input[i] == '\"' || input[i] == '\'') && foundQ == 0) {
            foundQ = 1;
            ++noTokens;
        } else if ((input[i] == '\"' || input[i] == '\'') && foundQ == 1) {
            foundQ = 0;
        }

        if (input[i] == ' ' && foundQ == 0) {
            ++noTokens;
        }
    }

    return noTokens;
}

//* returns an array of strings aka tokens 
//* they are delimited by a whitespace
char** tokeniser(char* input) {
    int len = strLen(input);
    int noTokens = countTokens(input, len);
    int* pos = tokensPos(input, len);
    char** tokens = malloc((noTokens + 2) * sizeof(char*));

    for (int i = 0; i < noTokens + 1; ++i) {
        int tokenLen = pos[i + 1] - pos[i];
        char* token = malloc(tokenLen * sizeof(char));

        for (int j = 0; j < tokenLen; ++j) {
            token[j] = input[pos[i] + j];
        }
        *(tokens + i) = token;
    }

    return tokens;
}

//* strip -> works with whitespace or \n at the end of strings
char* strip(char* input) {
    int inLen = strLen(input);
    char* token = malloc(inLen * sizeof(char));
    int counter = 0;

    for (int i = 0; i < inLen - 1; ++i) {
        if (input[i] != ' ' || input[i] != '\n' || input[i] != '\t' || input[i] != EOF) {
            token[counter] = input[i];
            ++counter;
        }
   }

    return token;
}

//* takes an char** array, char* command and an int* N array, returns char*
//* but this is only for removing the N and (N + 1) element
//? this is useful for when you want to run commands and strip
//?     the redirection symbols...
//> NOTE THAT EVEN FOR THE "<" SYMBOL, THE (N + 1) ELEMENT IS REMOVED
//>     because the commands will be appended to the command string
//? by the time this will be used all the edge cases like "<" at the start
//?    would have been handled
char* stripN(char** input, int* symbolPos) {
    int len = arrLen(input);

    int totalLen = 0;
    for (int i = 0; i < len; ++i) {
        totalLen += strLen(input[i]);
    }

    char* values = malloc((len + totalLen + 1) * sizeof(char));

    // for (int i = 0; i < len; ++i) {
    //     printf("%s\n", input[i]);
    // }
    // printf("=======\n");
    int checker = 0;
    for (int i = 1; i < len; ++i) {
        for (int j = 0; symbolPos[j] != ENDOFINTARR; ++j) {
            if (symbolPos[j] != '!' && (i == symbolPos[j] || i == symbolPos[j] + 1)) {
                checker = 1;
                break;
            }

            checker = 0;
        }

        if (checker == 0 && i != 1) {
            values = strcat(values, " ");
            values = strcat(values, strip(input[i]));
        } else if (checker == 0 && i == 1) {
            values = strcat(values, strip(input[i]));
        }
    }

    // for (int i = 0; i < len; ++i) {
    //     printf("%s\n", input[i]);
    // }
    // printf("---\n");
    return values;
}

// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.

// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.

// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.

// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study. 

// FILIP BUMBU, 21373616
