#include <iostream>
#include<bits/stdc++.h>
#include "header.h"
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
using namespace std;
void foregroundfun(vector<string> tokens){   
    pid_t pid = fork();
    if (pid == 0){ 
        char* args[tokens.size()+1];
        for(int i=0;i<tokens.size();i++){
            args[i]=const_cast<char*>(tokens[i].c_str());
        }
        args[tokens.size()]=NULL;

        if(execvp(args[0],args)==-1){
            cerr<<strerror(errno)<<endl;
            _exit(0);
        }        
        
    } else if (pid > 0) {
        int status;
        foreground_pid = pid;
        waitpid(pid,&status,2);       
        foreground_pid = -1; 
        return;
    } else {
        std::cerr << "Fork failed." << std::endl;
    }

    return;
}