#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include "header.h"

using namespace std;

string strip_quotes(const string& s) {
    if (s.size() >= 2 && 
        ((s.front() == '"' && s.back() == '"') || (s.front() == '\'' && s.back() == '\''))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

void pipefun2(int n, vector<vector<string>> pipecommands, int input_fd) {
    int pipefd[2];

    if (n < pipecommands.size() - 1) {
        pipe(pipefd); 
         }
    vector<string> command = pipecommands[n];
    string input_file = "", output_file = "", iosymbol = "";
    bool has_redirection = false;

    for (size_t i = 0; i < command.size(); ) {
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
            continue; // don't increment i here, because erase shifts elements
        }
        i++;
    }

    int output_fd = STDOUT_FILENO;
    if (n < pipecommands.size() - 1) {
        output_fd = pipefd[1];
    }

    char* args[command.size() + 1];
    for (size_t i = 0; i < command.size(); i++) {
        string arg = strip_quotes(command[i]);
        args[i] = strdup(arg.c_str()); // strdup to avoid issues with execvp
    }
    args[command.size()] = NULL;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { // Child
        // Input redirection
        if (!input_file.empty()) {
            int fd = open(input_file.c_str(), O_RDONLY);
            if (fd < 0) { perror("open input"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // Output redirection
        if (n == pipecommands.size() - 1 && !output_file.empty()) {
            int fd;
            if (iosymbol == ">>")
                fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror("open output"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (n < pipecommands.size() - 1) {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        }

        // Close unused pipe read end in child
        if (n < pipecommands.size() - 1) close(pipefd[0]);

        // for (int dbg = 0; args[dbg] != NULL; ++dbg) {
        //     cerr << "arg[" << dbg << "]: " << args[dbg] << endl;
        // }

        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else { // Parent
        waitpid(pid, nullptr, 0);

        if (input_fd != STDIN_FILENO) close(input_fd);
        if (n < pipecommands.size() - 1) {
            close(pipefd[1]);
            pipefun2(n + 1, pipecommands, pipefd[0]);
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

