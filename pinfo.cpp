#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include "header.h"
using namespace std;
void pinfofun(vector<string> tokens){
    string pidstr;
    pid_t pid;
    if(tokens.size()>1){
        pidstr=tokens[1];
        pid=stoi(tokens[1]);
    }else{
        pid=getpid();
        pidstr=to_string(pid);
        tokens.push_back(to_string(pid));
    }
    string processpath = "/proc/" + pidstr;
    ifstream statFile(processpath + "/stat");
    if (!statFile.is_open()) {
        cerr << "Failed to open " << processpath << "/stat" <<endl;
        return;
    }
    string statusLine;
    getline(statFile, statusLine);
    statFile.close();
    vector<string> vec;
    stringstream info(statusLine);
    string token;
    while(info >> token){
        vec.push_back(token);
    }

    string processstat = vec[2]; 
    pid_t pgid = stoi(vec[4]);     
    unsigned long vsize = stoul(vec[22]); 

    
    pid_t frgrndid = tcgetpgrp(STDIN_FILENO);
    
    if (pgid == frgrndid){
        processstat += "+";
    }
    char exepath[PATH_MAX];
    ssize_t len = readlink((processpath + "/exe").c_str(), exepath, sizeof(exepath) - 1);
    if (len != -1) {
        exepath[len] = '\0';
    } else {
        strcpy(exepath, "Unknown");
    }
    std::cout << "pid -- " << tokens[1] << endl;
    std::cout << "Process Status -- {" << processstat << "}" << endl;
    std::cout << "memory -- " << vsize << " {Virtual Memory}" << endl;
    std::cout << "Executable Path -- " << exepath << endl;
    return;
}