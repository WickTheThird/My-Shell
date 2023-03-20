#include "./myshell.h"
#include "./utility.c"

int batchMode(char** fileName);

//* ENTERS IN AN INFINITE LOOP
//* IT FIRSTLY GOES INTO A MYSHELDIR FILE WHERE EVERYTING IS HAPPENNING
//* NEXT ITS CHECKING FOR BATCH MODE AND PROCCEEDS ACCORDINGLY
int main(int argc, char *argv[]) {

    system("mkdir myshellDir");
    CD("myshellDir");

    if (argc == 1) {
        while(RUN) {
            // printf("startover\n");
            char** in = input(); // PROCESSES INPUT
            int inLen = arrLen(in);

            // START OF COMMAND CALLS
            int outer = caller(in, inLen); // CALLS THE COMMANDS

            // 0 -> EXIT
            // 1 -> OTHER
            if (outer == 0) {
                exit(1);
            }
        }
    } else {
        if (batchMode(argv) != 0) {
            printf("Error: batch mode failed\n"); // failsafe
            exit(1);
        }
    }

    return (0);
}


// TITLE: BATCH MODE
//* this function is used to run the commands from a file
//* it takes the file name as an argument
//* it then checks if the file exists and is readable
//* if it is, it opens the file and reads the commands
//* it then calls the command runner function
//* it returns 0 if everything is fine
//* it returns 1 if there is an error
//* it redirects command to commandrunner, 
//*   meaning it can process io redirection or background processing (in theory);
int batchMode(char** files) {

    char *bin_dir = ".";
    char *file_name;
    char *file_path;
    FILE *fp;
    char command[INPUT_SIZE];
    int ret = 0;

    for (int i = 1; files[i] != NULL; i++) {
        file_name = files[i];

        if (strcmp(file_name, "makefile") != 0 || strcmp(file_name, "myshell") != 0) {
            file_path = malloc(strlen(bin_dir) + strlen(file_name) + 1);
            strcpy(file_path, bin_dir);
            strcat(file_path, file_name);

            if (access(file_path, R_OK) != 0) {
                printf("Error: File '%s' not found or is not readable.\n", file_name);
                ret = 1;
                continue;
            }

            fp = fopen(file_path, "r");

            if (fp == NULL) {
                printf("Error opening file.\n");
                ret = 1;
                continue;
            }

            while (fgets(command, sizeof(command), fp) != NULL) {
                command[strcspn(command, "\n")] = 0;

                char** words = tokeniser(command);

                int found = 0;
                for (int i = 0; commands[i].name != NULL; i++) {
                    if (strcmp(commands[i].name, words[0]) == 0) {
                        int retcode = commands[i].func(*(words+1));
                        if (retcode != 0) {
                            printf("Error executing command '%s'\n", command);
                            ret = 1;
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    int symbolPos[2];
                    symbolPos[0] = ENDOFINTARR;
                    if (commandRunner(words, symbolPos) != 0) {
                        printf("Error executing command '%s'\n", command);
                        ret = 1;
                    }
                }

                free(words);
            }

            fclose(fp);
            free(file_path);
        }
    }

    return ret;
}

// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.

// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.

// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.

// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study. 

// FILIP BUMBU, 21373616
