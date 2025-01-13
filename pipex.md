# Pipex Project Guide

## Goal
The goal of the pipex project is to develop a program that simulates the behavior of pipes in the shell. Specifically, the program should reproduce the functionality of the following shell command:

< file1 cmd1 | cmd2 > file2

## Key Concepts

### Input Redirection (`<`)
Imagine you have a book (file) and you want to read it out loud (command).
Input redirection means you read the book and use its content for your task.
Example: grep a1 < infile means you read the book called infile and look for "a1" in it.

### Pipe (`|`)
Think of a pipe as a tube connecting two tasks.
The first task does something and sends its result through the tube to the second task.
Example: grep a1 < infile | wc -w means you find "a1" in the book and then count the words found, sending the result through the tube.

### Output Redirection (`>`)
Imagine you have a notebook (file) where you write down results.
Output redirection means you write the result of your task into the notebook, replacing its content.
Example: grep a1 < infile | wc -w > outfile means you count the words found and write the result in the notebook called outfile.

### Append Output Redirection (`>>`)
Similar to output redirection, but instead of replacing, you add the result to the end of the notebook.
Example: cmd >> file means you add the result of your task to the end of the notebook called file.

### Heredoc (`<<`)
Imagine you keep writing until you see a special word that tells you to stop.
Heredoc means you keep reading input until you see a specific word (LIMITER), then you use that input for your task.
Example: cmd << LIMITER | cmd1 >> file means you keep reading until you see "LIMITER", then send the input through the tube to another task and add the result to the notebook.


### Main checklist

Check the existence of infile and outfile


be sure to understand what > does when the file does not exist


Create the necessary pipe (or pipes)


Create a child process for each command


Wait for all the processes to end before writing to the outfile

When using here_doc, the second argument is not a command 😉

### Execute checklist

Remember that the execve() function needs the path to a binary file as parameter, so you'll have to find where the commands binaries are stored on your computer. Before going further, you have to know how to find any command binary.


Check in all possible locations if the binary (command) requested by the user exists.


"Build" the arguments array for the command.

Execute the command using execve()

## Steps to Implement Pipex

### 4. Implement Core Functionality

Special edge cases: /dev/urandom and /dev/stdin

#### Process Management
- Use `fork` to create child processes.
  - Pseudocode:
    ```
    pid = fork()
    if pid < 0:
        handle_error("Fork failed")
    if pid == 0:
        // Child process code
    else:
        // Parent process code
    ```

#### Pipes
- Use `pipe` for inter-process communication.
  - Pseudocode:
    ```
    int pipefd[2]
    if pipe(pipefd) == -1:
        handle_error("Pipe failed")
    ```

#### File Descriptors
- Use `dup2` to duplicate file descriptors for redirection.
  - Pseudocode:
    ```
    if dup2(pipefd[0], STDIN_FILENO) == -1:
        handle_error("dup2 failed")
    if dup2(pipefd[1], STDOUT_FILENO) == -1:
        handle_error("dup2 failed")
    ```

#### Execute Commands
- Use `execve` to execute commands.
  - Pseudocode:
    ```
    char *cmd[] = { "command", "arg1", "arg2", NULL }
    if execve(cmd[0], cmd, envp) == -1:
        handle_error("execve failed")
    ```

### 5. Handle Redirection
- Implement input and output redirection using `open`, `close`, `read`, and `write`.
  - Pseudocode:
    ```
    int infile = open("inputfile", O_RDONLY)
    if infile < 0:
        handle_error("Open input file failed")
    if dup2(infile, STDIN_FILENO) == -1:
        handle_error("dup2 failed")
    close(infile)

    int outfile = open("outputfile", O_WRONLY | O_CREAT | O_TRUNC, 0644)
    if outfile < 0:
        handle_error("Open output file failed")
    if dup2(outfile, STDOUT_FILENO) == -1:
        handle_error("dup2 failed")
    close(outfile)
    ```

### 6. Error Handling
- Check return values of system calls.
  - Pseudocode:
    ```
    if (system_call() == -1):
        handle_error("System call failed")
    ```

- Use `errno`, `perror`, and `strerror` for error reporting.
  - Pseudocode:
    ```
    void handle_error(const char *msg) {
        perror(msg)
        exit(EXIT_FAILURE)
    }
    ```
