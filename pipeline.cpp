#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include "header.h"

using namespace std;
void pipefun2(int n, vector<vector<string>> pipecommands, int input_fd) {
    int pipefd[2];

    if (n < pipecommands.size() - 1) {
        pipe(pipefd); 
         }
    vector<string> command = pipecommands[n];
    string input_file = "", output_file = "", iosymbol = "";
    bool has_redirection = false;

    for (size_t i = 0; i < command.size(); ++i) {
        if (command[i] == ">" || command[i] == "<" || command[i] == ">>") {
            has_redirection = true;
            iosymbol = command[i];
            if (command[i] == ">" || command[i] == ">>") {
                output_file = command[i + 1];
            }
            if (command[i] == "<") {
                input_file = command[i + 1];
            }
            command.erase(command.begin() + i, command.begin() + i + 2);
            break;
        }
    }

    if (has_redirection) {
        int output_fd = (n < pipecommands.size() - 1) ? pipefd[1] : STDOUT_FILENO;
        redirectionfun2(command, input_file, output_file, iosymbol, input_fd, output_fd);
    } else {
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
            if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            if (n < pipecommands.size() - 1) {
                dup2(pipefd[1], STDOUT_FILENO);  
                close(pipefd[1]);
            }

            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else { 
            waitpid(pid, nullptr, 0);

            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }

            if (n < pipecommands.size() - 1) {
                close(pipefd[1]);  
                pipefun2(n + 1, pipecommands, pipefd[0]);  
            }
        }
    }
}

void pipefun(vector<string> tokens) {
    vector<vector<string>> pipecommands;
    vector<string> temp;
    for (auto &i : tokens) {
        if (i != "|") {
            temp.push_back(i);
        } else {
            pipecommands.push_back(temp);
            temp.clear();
        }
    }
    pipecommands.push_back(temp);

    pipefun2(0, pipecommands, STDIN_FILENO);
}

