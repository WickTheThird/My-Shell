#include "./myshell.h"

//TITLE COMMANDS USED IN THE SHELL
char* PWD() {
    static char path[MAX_PATH];
    getcwd(path, MAX_PATH);

    return path;
}

//> CHANGES THE CURRENT DIRECTORY
//* DONE
int CD(char* input) {
    // printf("tried to change to %s\n", input);
    int isDir = 0;

    if (strcmp(input, " ") == 0 || strcmp(input, "") == 0 || strcmp(input, "\n") == 0 || strcmp(input, "~") == 0 || input == NULL) {
        int state = chdir("myshell/");

        if (state < 0) {
            return 2;
        } else {
            printf("Changed to %s\n", input);
            return 0;
        }
    } else {

        isDir = dirCheck(input);

        if (isDir == 0) {
            int state = chdir(input);

            if (state < 0) {
                return 2;
            } else {
                printf("Changed to %s\n", input);
                return 0;
            }

        }
    }

    return 2;
}

//> SHOWS THE FILES AND FOLDERS IN THE CURRENT DIRECTORY
//? THIS SHOULD RETURN A CHAR**
//* DONE
int dir(char* input) {
    DIR *d;
    struct dirent *dir;
    struct stat fs;
    int flag;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {

            flag = stat(dir->d_name, &fs);
            if( flag == -1) {
                printf("File error\n");
                exit(1);
            }

            printf("%X ",fs.st_mode);
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    return 0;
}

//> THIS SHOULD RETURN A CHAR* WITH THE INPUT IN CASE OF IOREDIRECTION, THUS A PASSWER WILL CHECK THAT
int ECHO(char* input) {

    if (input == NULL) {
        return 1;
    }

    printf("%s\n", input);

    return 0;
}

//> ENVIRON
//? PRINTS ALL THE ENVIRONMENT VARIABLES
//? IM TAKING INPUT INORDER TO AVOID ANY ERRORS
void env(char* input) {
    if (system("env") != 0) {
        if (system("printenv") != 0) {
            printf("No environment variables found\n");
        }
    }
}

//> CLR -> clears the screen note that clear also works...
//? IM TAKING INPUT INORDER TO AVOID ANY ERRORS
void cls(char* input) {
    if (system("clear") != 0) {
        printf("Error: clear did not work\n");
    }
}

//> THIS ENTERS IN A LOOP THAT LISTENS FOR ENTER, WHEN PRESSED THE LOOP ENDS
//? IM TAKING INPUT INORDER TO AVOID ANY ERRORS
void PAUSE(char* input) {
    printf("Press enter to continue...\n");
    while(1){
        char enter = getchar();
        if (enter == '\n') {
            break;
        }
    }
}

//> THIS OPENS UP THE MANUAL AND DISPLAYS ALL THE DEFINITIONS
int HELP(char* input) {
   char man_command[50] = "man ";
    strcat(man_command, input);
    int status = system(man_command);
    if (status != 0) {
        printf("Error: no manual entry found for %s\n", input);
        return 1;
    }
    return 0;
}

//TITLE: STORING THE ABOVE COMMANDS IN A STRUCTURE

typedef struct  {
    char* name;
    int (*func)(char*);
} Commands;

Commands commands[] = {
    {"echo", (void*)ECHO},
    {"cd", (void*)CD},
    {"dir", (void*)dir},
    {"cls", (void*)cls},
    {"environ", (void*)env},
    {"pause", (void*)PAUSE},
    {"help", (void*)HELP},
    {"ENDARR", (void*)ENDARR}
};

//* CHECK WHAT COMMANDS ARE IN THE INPUT
//> note that are this used to be abble to access the commands
//> IF whichPos == 1 -> it refers to the position of commands inside the commands array
//> IF whichPos == 2 -> it refers to the position of commands inside the input array
int* posCommands(char** input, int whichPos) {
    // printf("this was called\n");
    int len = arrLen(input);

    int* commandsPos = malloc((len + 1) * sizeof(int));
    int counter = 0;

    for (int i = 0; i < len; ++i) {
        for (int j = 0; strcmp(commands[j].name, ENDARR()) != 0; ++j ) {
            // printf("|%s|, |%s|\n", commands[j].name, strip(input[i]));
            if (strcmp(commands[j].name, strip(input[i])) == 0) {
                if (whichPos == 1){
                    commandsPos[counter] = j;
                } else if (whichPos == 2) {
                    commandsPos[counter] = i;
                }
                ++counter;
            }

        }
    }

    commandsPos[counter] = ENDOFINTARR;

    return commandsPos;
}

//* RUN COMMANDS -> INCLUDING THE SYSTEM ONES
//? THE GENERAL COMMAND SYSTEM LOOKS LIKE THIS 
//> COMMAND [ARGUMENT1] [ARGUMENT2] [ARGUMENT3] [IOSYMBOL] [FILE] [ARGUMENT] [ARGUMENT] [IOSYMBOL] [FILE] [BACKGROUND]
//> THE COMMAND IS ALWAYS THE FIRST ONE, THE FILE IS IOSYMBOL + 1, THE ARGUMENTS IS EVERYTING IN BETWEEN THAT WILL BE 
//>      PASSED TO THE SYSTEM...
int commandRunner(char** input, int* symbolPos) {
    int* commandPos = posCommands(input, 2);
    int* whichCommand = posCommands(input, 1);
    
    int bg = 0;
    if (symbolPos[0] != ENDOFINTARR && symbolPos[symbolPos[0]] == '&') {
        bg = 1;
        symbolPos[symbolPos[0]] = ENDOFINTARR;
    }

    if (commandPos[0] == ENDOFINTARR) {
        char* command = input[0];
        char* values;
        values = stripN(input, symbolPos);
        if (values != NULL) {
            command = strcat(command, " ");
            command = strcat(command, values);
        }
        if (system(command) != 0) {
            return 1;
        }
    } else {
        char* values;
        int flag = 0;
        values = stripN(input, symbolPos);
        if (values != NULL) {
            flag = commands[whichCommand[0]].func(values);
        }
        return flag;
    }

    if (bg) {
        pid_t pid = fork();
        if (pid == 0) {
            int result = system(input[0]);
            if (result != 0) {
                printf("Error: command failed to execute\n");
            }
            exit(0);
        } else if (pid < 0) {
            printf("Error: failed to fork process\n");
            return 1;
        } else {
            printf("Background process started with PID %d\n", pid);
            return 0;
        }
    }

    return 0;
}

// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.

// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.

// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.

// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study. 

// FILIP BUMBU, 21373616
