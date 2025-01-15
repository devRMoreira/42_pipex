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

### Pseudo

START
  PARSE command-line arguments
  IF number of arguments != 5 THEN
    PRINT usage message
    EXIT

  OPEN file1 for reading
  OPEN file2 for writing

  CREATE pipe

  FORK process for cmd1
    IF child process THEN
      REDIRECT stdin to file1
      REDIRECT stdout to pipe write end
      CLOSE pipe read end
      EXECUTE cmd1
    ELSE
      CLOSE pipe write end

  FORK process for cmd2
    IF child process THEN
      REDIRECT stdin to pipe read end
      REDIRECT stdout to file2
      CLOSE pipe write end
      EXECUTE cmd2
    ELSE
      CLOSE pipe read end

  WAIT for both child processes to finish

  CLOSE all file descriptors
  ENSURE no memory leaks

END


#### Initialize Program
Parse command-line arguments.
Check if the number of arguments is correct (should be 5).

#### Open Files
Open file1 for reading.
Open file2 for writing.

#### Create Pipes
Create a pipe to connect the output of cmd1 to the input of cmd2.

#### Fork Process for cmd1
In the child process:
Redirect the standard input to file1.
Redirect the standard output to the write end of the pipe.
Close the read end of the pipe.
Execute cmd1.
In the parent process:
Close the write end of the pipe.

#### Fork Process for cmd2
In the child process:
Redirect the standard input to the read end of the pipe.
Redirect the standard output to file2.
Close the write end of the pipe.
Execute cmd2.
In the parent process:
Close the read end of the pipe.

#### Wait for Child Processes
Wait for both child processes to finish.

#### Clean Up
Close all file descriptors.
Ensure no memory leaks.
