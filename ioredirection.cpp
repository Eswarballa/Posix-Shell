#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include "header.h"
using namespace std;
void redirectionfun2(vector<string> command, string input_file, string output_file, string iosymbol, int input_fd, int output_fd) {
    char* args[command.size() + 1];
    for (size_t i = 0; i < command.size(); i++) {
        args[i] = const_cast<char*>(command[i].c_str());
    }
    args[command.size()] = NULL;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { 
        if (!input_file.empty()) {
            int fdin = open(input_file.c_str(), O_RDONLY);
            if (fdin == -1) {
                perror("open input file failed");
                exit(1);
            }
            dup2(fdin, STDIN_FILENO);
            close(fdin);
        } else if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (!output_file.empty()) {
            int fdout;
            if (iosymbol == ">>") {
                fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fdout == -1) {
                perror("open output file failed");
                exit(1);
            }
            dup2(fdout, STDOUT_FILENO);
            close(fdout);
        } else if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

      execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else { 
        waitpid(pid, nullptr, 0);

        if (input_fd != STDIN_FILENO) close(input_fd);
        if (output_fd != STDOUT_FILENO) close(output_fd);
    }
}

void redirectionfun(vector<string> tokens, int input_fd = STDIN_FILENO, int output_fd = STDOUT_FILENO) {
    vector<string> command;
    string inputfile = "", outputfile = "";
    string iosymbol = "";
    int flag = 0;

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == ">" || tokens[i] == "<" || tokens[i] == ">>") {
            flag = 1;
            iosymbol = tokens[i];
            if (tokens[i] == ">" || tokens[i] == ">>") {
                outputfile = tokens[i + 1];
            }
            if (tokens[i] == "<") {
                inputfile = tokens[i + 1];
            }
            
            i++;
        } else if (flag == 0) {
            command.push_back(tokens[i]);
        }
    }

     redirectionfun2(command, inputfile, outputfile, iosymbol, input_fd, output_fd);
}