#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include "header.h"
using namespace std;

bool searchfun2(string path, string filename) {
    DIR* dirs = opendir(path.c_str());
    if (dirs == nullptr) {
        return false; 
    }
    struct dirent* thisdir;
    while ((thisdir = readdir(dirs)) != nullptr) {
        string dir_name = thisdir->d_name;
        string full_path = path + "/" + dir_name; 
        struct stat dir_stat;
        if (dir_name == "." || dir_name == "..") {
            continue;
        }
        if (stat(full_path.c_str(), &dir_stat) == -1) {
            cerr << "Failed to get stats for: " << full_path << " - " << strerror(errno) << endl;
            continue;
        }
        if (dir_name == filename) {
            closedir(dirs); 
            return true;
        }
        if (S_ISDIR(dir_stat.st_mode)) {
            if (searchfun2(full_path, filename)) {
                closedir(dirs);  
                return true;
            }
        }
    }
    closedir(dirs); 
    return false;
}

bool searchfun(vector<string> tokens) {
    char path[100];

    if (tokens.size() < 2) {
        cerr << "Error: Missing filename argument." << endl;
        return false;
    }
    if (getcwd(path, 100) == nullptr) {
        cerr << "Error getting current working directory path: " << strerror(errno) << endl;
        return false;
    }
    string currpath = string(path); 
    return searchfun2(currpath, tokens[1]);
}

