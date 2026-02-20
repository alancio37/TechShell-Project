#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>


// Function to remove the newline character at the end of the input string
void stripNewline(char *input) {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0'; // Remove the newline
        }
    }

// Get the current working directory
void currentWD(){
    // allocate memory for our path
    char workDir[1024];
    //
    if (getcwd(workDir, sizeof(workDir)) != NULL){
        printf("%s\n", workDir);
    } else {
        perror("getcwd");
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

                    //////FINISH CD COMMANDS//////

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

// Processes Function
void filePRS(){

}

// Copy a file to a destination
void fileCOPY(char *filename, char *destination){
    char buffer[1024];
    FILE *file = fopen(filename, "r");
    FILE *goal = fopen(destination, "w");

    if (file == NULL || goal == NULL){
        perror("Invalid input\n");
    }

    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytesRead, goal);  // Write the data to the destination file
    }

    fclose(file);
    fclose(goal);

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
        printf("Directory removed\n");
    } else {
        printf("The specified directory may have contents within]\n");
}
}

// Make a directory
void makeDir(char *dirName){
    
    int val = 0;
    val = mkdir(dirName, 0755);

    if (val == 0){
        printf("Directory created\n");
    } else {
        printf("Cannot createa directory %s\n", dirName);
    }
}

// Create a file
void fileRMV(char *filename){
    if (remove(filename) == 0){
        printf("File removed\n");
    }
}


void exitCmd(){
    // end the terminal
    exit(0);
}

int commandExec(char *input){
       // Strip the newline character before processing
    stripNewline(input);

    char *command = strtok(input, " \n");
    if (command == NULL){
        printf("Invalid Command\n");
        return 0;
    }

    // If command string is pwd
    //  then run the getcwd function
    if (strcmp(command, "pwd") == 0){
        currentWD();
        return 1;
    }

    if (strcmp(command, "ls") == 0) {
        // Check for an argument for "ls"
        char *arg = strtok(NULL, " \n");  // Get the argument after "ls"
        if (arg == NULL) {
            lsCom(".");  // Default to current directory if no argument
        } else {
            lsCom(arg);  // List the contents of the directory passed as argument
        }
        return 1;
    }

    // If command is "exit" then run 
    //  the exit function or if the 
    //    command is invalid for some reason,
    //      print the error message
    if (strcmp(command, "exit") == 0){
        exitCmd();
        return 0;
    } else {
        printf("Error 2 (no such file or directory)\n");
    }

    // If the "chmod <argument>" is entered
    //  then initiate the CHMOD function
    if (strcmp(command, "chmod") == 0) {
    char *mode = strtok(NULL, " \n");
    char *file = strtok(NULL, " \n");

    if (mode && file) {
        filePerms(mode, file);
    } else {
        printf("Invalid input for chmod\n");
    }
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
    }

    // If "cp <file-name> <destination>" is entered, copy 
    //  a file to the specified destination
    if (strcmp(command, "cp") == 0){
        char *dest = strtok(NULL, " \n");
        char *file = strtok(NULL, " \n");

        if (dest == NULL || file == NULL){
            perror("One or more inputs were invalid\n");
        } else{ 
            fileCOPY(file, dest);
        }
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
        }
        return 1;

}

void shell_loop(){

    char *input = NULL;
    ssize_t input_size = 0;

    while(1){
        printf("~$: ");
        if (getline(&input, &input_size, stdin) == -1)
            break;

        if (!commandExec(input)){
            break;
        } 
    }

    // free the memory!
    free(input);

}

// third input gives us the environment 
int main(){ 

    shell_loop(); 

    return 0; 
}