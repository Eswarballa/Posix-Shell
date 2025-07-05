
# POSIX Shell Implementation

implemented own posix shell like bash using c++,commands implemented echo, pwd, cd, ls, foreground-background process, pinfo, search, ioredirection, pipeline, history, tab autocomplete uparrow and downarrow recommendations.

this shell allows you to run any implementable command using execuvp for the commands which i have not implemented like mkdir,clear etc

## implementation instriction 
1.run make command on terminal where file exists 


2.run ./a.out to start terminal


3.now terminal is ready 


## main.cpp 

print the hostname and systemname using environment variables in an infinite loop which takes command charecter by charecter and sends it to userinp.cpp function after tokenising with ";" for processing .while entering charecter we will check for special charecters like tabspace, ctrl C, Z, A, uparrow and downarrow if not we will append charecters to string 

## makefile
write implementation code starting with g++ and all file names with it to compile all files at once

## header.h
include all functions in header.h and include header.h header in all remaining files to acess any function anywhere

## userinp.cpp
tokensize the input with spaces and send tokens to their related function get the current path initially because to check whether path is changed or not and print path if changed.pwd is implemented in this file itself using getpwd system call

## echo.cpp
remove all white spaces while tokenising and append each token with trailing space and print it on console

## cd.cpp
userinp functioncalls cd function after checking first token cd.cpp file change the current directory and return after function call current path will be fetched and printed in console

## ls.cpp
userinp function calls ls function after checking first token ls.cpp will group the given flags and execute it for all directories.if the given name is file name then it calls filels function and print the output in console

## foreground.cpp & background,cpp
if there is an '&' at last then that command redirects to background and it executes in background by setting its pgid to 0
if there is no & and none of the writen command matches the command it goes to foreground and executed by execvp

##pinfo.cpp
we get the string from the proc file in user/bin and get all the details about process by tokenising the string we got from proc file and we check whether process is foreground or background by pgid and pgrpid

## search.cpp
search function search for file or folder if found then return true else false search performs dfs throughout its tree below

## ioredirection
if ioredirection implemented by checking input and output file and reading from input and writing to output

## pipeline
pipeline implemented using pipe function and dup2 systemcall which redirects the output to file discriptor 

## pipeline with ioredirection
if there is redirection during execution of pipeline it calls redirection function and passes pipe end as filediscriptor to read from it and write it on another pipe

## simple signals
implemented signals in main function while reading charecter by charecter
ctrl-Z //pushes foreground to background
ctrl-c //foreground running process kill
ctrl-D //exit 

## Autocomplete for all the files/directories
if tab is pressed then it invokes tabsuggestion function where it returns longest prefix matching functions if it ia already prefix then prints possible outputs

## history.cpp
maintain a queue and history.txt , loads the content to queue form history.txt when execution starts and write to file history.txt from queue when execution ends.queue can store only 20 commands and file can consist 20 commands

## Up Arrow Key
maintain a integer that increments on uparrow and decrements on downarrow which will initialize command on queue from kth posistion 



