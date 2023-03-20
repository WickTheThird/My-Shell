#include "./myshell.h"
#include "./process.c"
#include "./commands.c"

//TITLE: IO REDIRECTION AND BACKGROUND PROCESSING

//> STANDARD OUTPUT ">"
//? the function executes the command, then it writes the output to the file
//? it then redirects the output to the file
//? it then closes the file
void rStdout(char** input, int symbolPos, int* symbolsPos, int count) {
    // printf("entered rStdout\n");
    int outfd;
    char* fileName = strip(input[symbolPos + 1]);

    outfd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY , 0666);

    if (outfd < 0) {
        printf("Error: failed to open %s\n", fileName);
        return;
    }

    int saved_stdout = dup(STDOUT_FILENO);

    if (dup2(outfd, STDOUT_FILENO) < 0) {
        printf("Error: failed to redirect stdout\n");
        close(outfd);
        return;
    }

    close(outfd);

    int flag = commandRunner(input, symbolsPos);
    printf("\n");

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    if (flag == 1) {
        printf("Error: no such command\n");
        return;
    } else if (flag == 2) {
        printf("Error: no such file or directory\n");
        return;
    }

    // printf("exited rStdout\n");
}


//> STANDARD INPUT "<"
//? the function reads the file, then it is appended to the end of the system run in the command
void rStdin(char** input, int symbolPos, int* symbolsPos, int count) {
    char* fileName = strip(input[symbolPos + 1]);

    int in_fd = open(fileName, O_RDONLY);
    if (in_fd < 0) {
        printf("Error: failed to open %s\n", fileName);
        return;
    }

    int saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin < 0) {
        printf("Error: failed to save stdin\n");
        close(in_fd);
        return;
    }

    if (dup2(in_fd, STDIN_FILENO) < 0) {
        printf("Error: failed to redirect stdin\n");
        close(in_fd);
        close(saved_stdin);
        return;
    }

    close(in_fd);

    char* command = strip(input[0]);
    char* values;

    values = stripN(input, symbolsPos);
    if (values != NULL) {
        command = strcat(command, " ");
        command = strcat(command, values);
    }

    if (system(command) != 0) {
        printf("Error: failed to execute command\n");
    }

    if (dup2(saved_stdin, STDIN_FILENO) < 0) {
        printf("Error: failed to restore stdin\n");
    }

    close(saved_stdin);
}


//> APPEND TO FILE ">>"
//? the function executes the command, then it appends the output to the file
void rAppend(char** input, int symbolPos, int* symbolsPos, int count) {
    // printf("entered rAppend\n");
    int outfd;
    char* fileName = strip(input[symbolPos + 1]);

    outfd = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0666);

    if (outfd < 0) {
        printf("Error: failed to open %s\n", fileName);
        return;
    }

    int saved_stdout = dup(STDOUT_FILENO);

    if (dup2(outfd, STDOUT_FILENO) < 0) {
        printf("Error: failed to redirect stdout\n");
        close(outfd);
        return;
    }

    close(outfd);

    int flag = commandRunner(input, symbolsPos);
    printf("\n");

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    if (flag == 1) {
        printf("Error: no such command\n");
        return;
    } else if (flag == 2) {
        printf("Error: no such file or directory\n");
        return;
    }
    // printf("exited rAppend\n");
}


//> BACKGROUND PROCESSING "&"
//? the function executes the command, then it writes the output to a file
void runInBackground(char** input) {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error: failed to fork process\n");
        exit(1);
    } else if (pid == 0) {
        // Redirect input from /dev/null to prevent child process from blocking
        int fd = open("/dev/null", O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);

        // Redirect output to file
        char output_file[20];
        sprintf(output_file, "output%d.txt", getpid());
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (output_fd == -1) {
            printf("Error: failed to open output file\n");
            exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        dup2(output_fd, STDERR_FILENO);
        close(output_fd);

        // Execute command in background
        if (commandRunner(input, NULL) != 0) {
            printf("Error: command failed to execute\n");
            exit(1);
        }

        exit(0);
    } else {
        // Parent process: print background process PID
        printf("Background process started with PID %d\n", pid);
    }
}

//> RUNS COMMANDS IN THE BACKGROUND
//* This function is called when the user enters a command with the '&' symbol
//* It forks the process, then redirects the output to a file
//* The parent process prints the PID of the child process
void rBackground(char** input, int symbolPos, int* symbolsPos, int count) {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error: failed to fork process\n");
        exit(1);
    } else if (pid == 0) {
        // Redirect input from /dev/null to prevent child process from blocking
        int fd = open("/dev/null", O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);

        // Redirect output to file
        char output_file[20];
        sprintf(output_file, "output%d.txt", getpid());
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (output_fd == -1) {
            printf("Error: failed to open output file\n");
            exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        dup2(output_fd, STDERR_FILENO);
        close(output_fd);

        // Execute command in background
        commandRunner(input, symbolsPos);

        exit(1);
    } else {
        // Parent process: print background process PID
        // printf("Background process started with PID %d\n", pid);

        // Wait for child process to complete
        int status;
        waitpid(pid, &status, 0);

        // Print content of output files
        char output_file[20];
        sprintf(output_file, "output%d.txt", pid);
        int output_fd = open(output_file, O_RDONLY);
        if (output_fd == -1) {
            printf("Error: failed to open output file\n");
            exit(1);
        }
        char buffer[1024];
        int n;
        while ((n = read(output_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);
        }
        close(output_fd);

        // Redirect stdout back to the command line
        dup2(STDOUT_FILENO, STDERR_FILENO);
        dup2(STDOUT_FILENO, STDOUT_FILENO);

        // Delete output files
        remove(output_file);
    }
}


//> STORING THE ABOVE SYMBOLS IN A STRUCTURE
typedef struct {
    char* name;
    void (*func)(char**, int, int*, int);
} Symbols;

Symbols symbols[] = {
    {">", (void*)rStdout},
    {"<", (void*)rStdin},
    {">>", (void*)rAppend},
    {"&", (void*)rBackground},
    {"ENDARR", (void*)ENDARR},
};


//TITLE: SYMBOL PROCESSING

//* CHECK WHAT SYMBOLS ARE IN THE INPUT
//> note that are this used to be abble to access the symbols for io red
//> IF whichPos ==  1 -> it refers to the position of symbols inside the symbols array
//> IF whichPos ==  2 -> it refers to the position of symbols inside the input array
int* posSymbols(char** input, int whichPos) {
    int len = arrLen(input);
    int totalLen = 0;

    for (int i = 0; i < len; ++i) {
        totalLen += strLen(input[i]);
    }

    int* symbolsPos = malloc((totalLen + 2) * sizeof(int));
    int counter = 0;

    for (int i = 0; i < len; ++i) {
        for (int j = 0; strcmp(symbols[j].name, ENDARR()) != 0; ++j ) {

            if (strcmp(symbols[j].name, strip(input[i])) == 0) {
                if (whichPos == 1) {
                    symbolsPos[counter] = j;
                } else if (whichPos == 2) {
                    symbolsPos[counter] = i;
                }
                ++counter;
            }

        }
    }

    symbolsPos[counter] = ENDOFINTARR;


    return symbolsPos;
}

//* CHECK IF THE SYMBOLS ARE RIGHT NEAR TO EACHOTHER OR THAT THE OF THE INPUT
//> note that these should work only with the following: >, <, 2>, >>
int checkSymbolPos(int* posSymbols, int* whichSymbols, int inputLen) {

    // for (int i = 0; posSymbols[i] != ENDOFINTARR; ++i) {
    //     printf("%d -> %s\n", posSymbols[i], symbols[whichSymbols[i]].name);
    // }

    for (int i = 0; posSymbols[i] != ENDOFINTARR; ++i)  {
        // printf("%d CHARACTER, %d INPUTLEN, %d SYMBOLCHECK\n", posSymbols[i], inputLen - 1, strcmp(symbols[whichSymbols[i]].name, "&"));
        if (posSymbols[i] == (inputLen - 1) && strcmp(symbols[whichSymbols[i]].name, "&") != 0) {
            printf("Error: parse error\nMake sure symbols are not at the end of the input or right next to eachother!\n");
            return 1;
        }

        if (posSymbols[i + 1] != ENDOFINTARR) {
            if (posSymbols[i + 1] - posSymbols[i] == 1 && 
            (strcmp(symbols[whichSymbols[i + 1]].name, "&") == 1 || strcmp(symbols[whichSymbols[i]].name, "&") != 0)) {
                return 1;
            }
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
