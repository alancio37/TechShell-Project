#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

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

// List the contents of a directory
void lsCom(const char *dir) {

    struct dirent *d;
    DIR *dh = opendir(dir);

    if (!dh) {
        return;
    }

    int found = 0;
    while ((d = readdir(dh)) != NULL) {
        // Skip "." and ".."
        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
            printf("%s\n", d->d_name);  // Print the file or directory name
            found = 1;
        }
    }

    if (!found) {
        printf("The directory is empty or no files found\n");
    }

    closedir(dh);
}

// CHMOD function
void filePerms(char *modeStr, char *file){

    char *endptr;

    mode_t modeType = strtol(modeStr, &endptr, 8);

    if (*endptr != '\0'){
        fprintf(stderr, "Invalid input: %s\n", modeStr);
        return;
    }

    if (chmod(file, modeType) == -1){
        perror("chmod failed");
    }
}

// Create a file
void fileCRT(char *filename){

    FILE *fp = fopen(filename, "a");

    if (fp == NULL){
        perror("Invalid\n");
    }

    fclose(fp);
}

// Remove a directory
void dirRMV(char *dir){

    if (rmdir(dir) == 0){
    } else {
        printf("The specified directory may have contents within or does not exist\n");
}
}

// Make a directory
void makeDir(char *dirName){
    
    int val = 0;
    val = mkdir(dirName, 0755);

    if (val == 0){
    } else {
        printf("Cannot createa directory %s\n", dirName);
    }
}

// Remove a file
void fileRMV(char *filename){

    if (remove(filename) == 0){
    } else {
        printf("File cannot be removed or does not exist\n");
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

    // If command string is pwd
    //  then run the getcwd function
    if (strcmp(command, "pwd") == 0){
        currentWD();
        return 1;
    }

    // If the command enter is "ls" or
    //  "ls <argument>"
    if (strcmp(command, "ls") == 0) {
        // Check for an argument for "ls"
        char *arg = strtok(NULL, " \n");  // Get the argument after "ls"
        if (arg == NULL) {
            lsCom(".");  // Default to current directory if no argument
        } else {
            lsCom(arg);  // List the contents of the directory passed as argument
        }
        return 1; // So fork
    }

    // If command is "exit" then run 
    //  the exit function or if the 
    //    command is invalid for some reason,
    //      print the error message
    if (strcmp(command, "exit") == 0){
        exitCmd();
        return 1;
    } 

    // If the "chmod <argument>" is entered
    //  then initiate the CHMOD function
    if (strcmp(command, "chmod") == 0) {
    char *mode = strtok(NULL, " \n");
    char *file = strtok(NULL, " \n");

    if (mode && file) { // If the user enters both necessary inputs
        filePerms(mode, file);
        } else {
        perror("Invalid input for chmod\n");
        }
        return 1;
    }   

    // If "mkdir <file-name>" is entered,
    //  then initiate the makeDir function
    if (strcmp(command, "mkdir") == 0){
        char *arg = strtok(NULL, " \n");

        if (arg == NULL){
            perror("That directory could not be created\n");
        } else {
            makeDir(arg);
        }
        return 1;
    }

    // if "touch <filename.type> is entered,
    //  create a file within the current directory
    if (strcmp(command, "touch") == 0){
        char *arg = strtok(NULL, " \n");

        if (arg == NULL){
            perror("File could not be created\n");
        } else {
            fileCRT(arg);
        }
        return 1;
    }

    // If "rmdir <specified_directory>" is entered,
    //  then remove that specified directory
    if (strcmp(command, "rmdir") == 0){
        char *arg = strtok(NULL, " \n");

        if (arg == NULL){
            perror("Could not remove that directory\n");
        } else {
            dirRMV(arg);
        }       
        return 1;
    }

    // If "rm <file-name>" is entered, then remove that
    //  file from where it is
    if (strcmp(command, "rm") == 0){
        char *arg = strtok(NULL, " \n");

        if (arg == NULL){
            perror("Invalid input\n");
        } else {
            fileRMV(arg);
        }
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

    char *args[100];
    int i = 0;

    args[i++] = command;

    char *arg;
    while ((arg = strtok(NULL, " \n")) != NULL){
        args[i++] = arg;
    }

    args[i] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
        // Child
        execvp(command, args);
        perror("Command not found");
        exit(1);
    } else if (pid > 0){
        wait(NULL);
    } else {
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

        printf("%s$: ", cwd);
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