#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <cstring>
using namespace std;

char prevpath[100] = "";  
char currpath[100] = ""; 

void cdfun(vector<string> tokens) {
    char path[100];
    if (getcwd(currpath, 100) == NULL) {
        cout << "Error getting current directory" << endl;
        return;
    }  
    if (tokens.size() == 1) {
        return;
    }  
    if (tokens.size() > 2) {
        cout << "bash: cd: too many arguments" << endl;
        return;
    }    
    if (tokens[1][0] == '/' && tokens.size() == 2) {
        getcwd(prevpath, 100); 
        if (chdir(tokens[1].c_str()) != 0) {
            cout << "bash: cd: " << tokens[1] << ": No such file or directory" << endl;
            return;
        }
        getcwd(currpath, 100); 
    }
    
    else if (tokens[1] == "~") {
        const char* homeDir = getenv("HOME");
        if (homeDir != nullptr) {
            getcwd(prevpath, 100);
            if (chdir(homeDir) != 0) {
                cout << "bash: cd: " << tokens[1] << ": No such file or directory" << endl;
                return;
            }
            getcwd(currpath, 100);
        } else {
            cout << "bash: cd: " << tokens[1] << ": HOME not set" << endl;
            return;
        }
    }
    
    else if (tokens[1] == "-") {
        if (strlen(prevpath) == 0) {
            cout << "bash: cd: OLDPWD not set" << endl;
            return;
        }
        cout << prevpath << endl;  
        char temp[100];
        strcpy(temp, currpath);  
        if (chdir(prevpath) != 0) {
            cout << "bash: cd: " << prevpath << ": No such file or directory" << endl;
            return;
        }
        getcwd(currpath, 100);  
        strcpy(prevpath, temp);  
    }
    else if (tokens[1] == "..") {
        getcwd(prevpath, 100);
        if (chdir("..") != 0) {
            cout << "bash: cd: ..: No such file or directory" << endl;
            return;
        }
        getcwd(currpath, 100);
    }
    else {
        getcwd(prevpath, 100);
        string relativePath = string(currpath) + "/" + tokens[1];
        if (chdir(relativePath.c_str()) != 0) {
            cout << "bash: cd: " << tokens[1] << ": No such file or directory" << endl;
            return;
        }
        getcwd(currpath, 100);
    }
}