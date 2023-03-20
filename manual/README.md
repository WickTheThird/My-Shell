# CA216 Operating Systems: myshell

## Introduction

This is the starter repository for the _ca216 Operating Systems_ project entitled `myshell` and which you must complete individually.

## What you need to do


You should follow the instructions below:

1. fork (do not clone) this repository.  Once you have forked it, make your own repository private and add both me (Graham Healy - @healygr) and Wandri Jooste (@joostew2) as "maintainer".
2. add all your source files to the `src` directory.
3. Add your files relating to the manual/help command to the `manual` directory.
4. Your `makefile` should build the binary and place it in the `bin` directory.

## Other notes
There are supporting exercises to help you with the project.  You will find them all under  [Lab04/05 on the Loop CA216 Site](https://loop.dcu.ie/mod/book/view.php?id=2054177).  Use Git regularly and commit versions of your code often.


THESE ARE A FEW OF MY OWN NOTES PLEASE CONSIDER WHAT IS TO CONSIDER FROM THEM
HERE THERE WILL BE SOME CONCERNS I HAD WHILE DOING THE SHELL OR CONFUSIONS

TITLE -> PROCESS
[COMMAND] [VALUE1] [VALUE2] [VALUE3] ... [VALUEN]

[COMMAND] -> the command to be executed (echo, cd, dir, pause, help, quit) -> those commands are build in;

STEP1: TOKENISE THE INPUT //> DONE
        -> the tokenisation is made by splitting the input into individual words;
        -> i know scanf() already does this for us, but then i have reached the conclusion
            that it is not very safe, this I have decided to create my own function implementing
            this. //! i have changed tp getf() and implemented a tokeniser...
        -> after the input has been tokenised, we can then pass it to the next part of our program
            which is the caller function.
STEP2: CALLER //> DONE
        -> in the caller we then must check if there is the correct number of inputs,
            that being the minimum of 1 input (the command itself).
        -> the caller also checks for the exit, note that it wasn't specified wether the exit command
            should be stanalone or not, so I have decided to make it both.
STEP3: VALUES //? DONE
        -> then, since inputn is tokenised we must then create some checkers, we must perticularly check for
            ">", "<" and "&", since those are the ones specified in the assignment.
        -> note that in the spec it wasn't specified wether the commandline should compute "&" between two 
            processes, thus I have decided to implement it such that it works for one process, thus pattern
            will always be [COMMAND] [VALUE1] [VALUE2] [&] //! if ths changes I will make sure to refer to it.
STEP4: COMMANDS //> DONE
        -> givent that the values have been processed, we then can execute the commands.
STEP5: BATCH MODE //> DONE
        -> the batch mode is the mode in which the program will be executed with a file as input.
        -> there are a number of things from the spec that i found a bit confusing, for example, 
            it wasn't specified where the batch files are expected to be found, thus I will place them
            in the bin directory. If a file is not there, then the program will exit with an error.
            secondly, it wasn't specified wether ther would be multiple files or not, thus I have decided to implement
            it such that it will take multiple files and execute them one by one.
OTHER: -> I have seen too late that i need my own ebvorionment for the shell, i have included a directory where
            the program will be executed, but I dont't know if that will be enough, sorry.


// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.

// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.

// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.

// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study. 

// FILIP BUMBU, 21373616
