Alancio Young

1. Group members: Alancio Young

2. My reponsibilities included researching and gathering information about ceratin
    functions/commands and how they worked, how I can incorporate those into my program, and putting the program together in general.

3. The project is a simple bash shell written in C that performs basic actions to files.
    Some of those things may include, and not be limited to, mkdir, touch, rmdir, rm, and chmod. Although not these commands may not be implemented in the most detailed way, they perform their intended use at base level like they would in an actual bash terminal.

4. For information on how to use the file >
    1. First in your terminal (and also ensuring you are in the corect directory)
        -> type "gcc techshell.c -o techshell" to compile the file.

    2. Second, after successfully compiling the file, enter -> "./techshell" into the 
        terminal to run the file

    3. From then on, enter some basic commands and their respective arguments (if        required) to see if they work (and all the ones listed above should)

5. To test..
    Cd:
     1. "cd" (will default to home directory)
     2. "cd .." (will move up one directory)
     3. "cd ~" (will go to the home directory)
     4. "cd destination" (will move to specified directory)

    ls:
     5. "ls" or "ls arg" (will list the contents of the directory you're currently in or specified)

    chmod:
     6. "chmod permission-numbers" (will change the file permission to whatever r, w, x number combination entered)

    pwd:
     7. "pwd" (prints the current working directory)

    mkdir:
     8. "mkdir directory-name" (makes the specified directory in the current directory)

    touch:
     9. "touch file-name" (makes a file in the current directory)

    rmdir:
     10. "rmdir directory-name" (removes the specified directory from the current directory)

    rm:
     11. "rm file-name" (removes the given file if it is in the current directory)

6. Resources:
    1. geeksforgeeks.org -> for function and implementation help (ex. chdir)
    
    2. delftstack.com -> for function and implementation help (ex. getcwd)

    3. iq.opengenius.org -> for function and implementation help (ex. ls)

    4. w3schools.org -> for more information on how said bash commands work and how they're
                         formatted function wise (I researched all functions I used and     ones I did not wind up implementing, like the copy command, using this)

    5. chatgpt.com -> for significant help on commands I found harder to implement 
                        (mainly chmod and the forking part for invalid inputs) as well as structuring and figuring out errors that I could not seem to fix myself.
                            For example: Although I eventually solved the error myself, when my program was having issues running my "exit" and "ls" commands, I would mainly ask commands with hypotheicals on why something could be happening given specific lines and when I could not figure that out, I would then just give it small snippets of my code and then go from there. 