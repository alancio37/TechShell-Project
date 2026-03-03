#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

// Alancio Young

// Function to remove the newline character at the end of the input string
void stripNewline(char *input) {

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0'; // Remove the newline
        }
    }

// Get the current working directory
char* currentWD() {
    static char workDir[1024];  
    if (getcwd(workDir, sizeof(workDir)) != NULL) {
        return workDir;  
    } else {
        perror("getcwd");
        return NULL;  
    }
}

// Change directory (move up one)
void changeDRC(){

    if (chdir("..") != 0) {
        perror("chdir");
        return;
    }
    currentWD();
}

// Change directory (home)
void changeDRCII(){

    char *home = getenv("HOME");
    if (home == NULL) {
        printf("HOME environment variable not set\n");
        return;
    }
    if (chdir(home) != 0) {
        perror("chdir");
        return;
    }
    currentWD();
}

// Change to a specificed directory
void changeDRCIII(const char *spcdir){
    if (chdir(spcdir) != 0){
        perror("chdir");
    } else {
        currentWD();
    }
}

// Exit the terminal
void exitCmd(){
    exit(0);
}

int commandExec(char *input){
    // Strip the newline character before processing
    stripNewline(input);

    // Parse the input from the argument to see
    //  if the there is a typed command
    //     *If nothing is typed, then the command is invalid
    char *command = strtok(input, " \n");
    if (command == NULL){
        perror("Invalid Command\n");
        return 0;
    } 

    // If command is "exit" then run 
    //  the exit function or if the 
    //    command is invalid for some reason,
    //      print the error message
    if (strcmp(command, "exit") == 0){
        exitCmd();
        return 1;
    } 

    // If command string starts with "cd"
    //  then run one of the following commands 
    //    depending on what is or is not entered 
    //      afterwards
    if (strcmp(command, "cd") == 0){
        char *arg = strtok(NULL, " \n");

        if (arg == NULL){
            changeDRCII(); // No arguments goes to home directory
            return 1;
        } else if (strcmp(arg, "..") == 0){ // "cd .." moves up one directory
            changeDRC();
            return 1;
        } else if (strcmp(arg, "~") == 0){ // "cd ~" moves to the home directory
            changeDRCII();
            return 1;
        } else {
            changeDRCIII(arg); // "cd <arg>" moves to whatever directory is specified
        }
        return 1;
        }

    // For the purpose of stdin and stdout
    char *args[100];
    int i = 0;

    char *file1 = NULL;
    char *file2 = NULL;

    args[i++] = command;

    char *arg;
    // If there's argument parsing after the command
    while ((arg = strtok(NULL, " \n")) != NULL){
        // Checks to see if the < (stdin) was entered
        if (strcmp(arg, "<") == 0){
            // If so, parse the argument after that
            file1 = strtok(NULL, " \n");
        } 
        // Checks if the > (stdout) was entered
        else if (strcmp(arg, ">") == 0){
            // If so, parse the argument entered after that
            file2 = strtok(NULL, " \n");
        } else {
            //
            args[i++] = arg;

        }
    }

    // Forking stuff
    args[i] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
        // Input redirection
        if (file1 != NULL){
            int file_1 = open(file1, O_RDONLY);
            if (file_1 < 0){
                perror("open input");
                exit(1);
            }
            dup2(file_1, STDIN_FILENO);
            close(file_1);
        }

        // Output redirection
        if (file2 != NULL){
            int file_2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file_2 < 0){
                perror("open output");
                exit(1);
            }
            dup2(file_2, STDOUT_FILENO);
            close(file_2);
        }


        // Child process
        // Gets the command and arguments entered and
        //  runs the appropriate bash commands
        execvp(command, args);
        // If the user input is invalid, return the error number using
        //  errno
        fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
        exit(1);
    } else if (pid > 0){
        // Parent process
        // Waits for the child process to finish before proceeding
        wait(NULL);
    } else {
        // Error message for if forking fails
        perror("fork failed");
    }    
    return 1;
}

// Loops that keeps the shell running
void shell_loop(){

    char *input = NULL;
    ssize_t input_size = 0;

    while(1){
        char *cwd = currentWD();

        printf("%s $: ", cwd);
        // Buffer, size of the buffer, stdin reads the line 
        //  getline() automatically resizes the buffer to the 
        //    necessary size
        //  *If the user presse "Ctrl + D" exits the loop.
        if (getline(&input, &input_size, stdin) == -1)
            break;

        // If the user uses the "exit" command, breaks said loop
        if (!commandExec(input)){
            break;
        } 
    }

    // free the memory!
    free(input);

}

// Main section that executes the program
int main(){ 

    shell_loop(); 

    return 0; 
}