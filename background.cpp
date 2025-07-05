#include <iostream>
#include<bits/stdc++.h>
#include "header.h"
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
using namespace std;

void backgroundfun(vector<string> tokens){
    pid_t pid = fork();
    if (pid == 0){
        if(setpgid(0,0)<0){exit(1);}
        char* args[tokens.size()+1];
        for(int i=0;i<tokens.size()-1;i++){
            args[i]=const_cast<char*>(tokens[i].c_str());
        }
        args[tokens.size()]=NULL;
        
        if(execvp(args[0],args)==-1){
            cout<<strerror(errno)<<endl;
            exit(0);
        }
    } else if (pid > 0) {
        cout<<pid<<endl;
        return;
    } 
    return;
}