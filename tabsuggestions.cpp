#include<bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>
#include "header.h"
using namespace std;


string filesuggestionfun(string &line){
    vector<string> tokens;
    stringstream commands(line);
    string token;
    while(commands >> token){
        tokens.push_back(token);
    }
    string lasttoken=tokens[tokens.size()-1];

    string prefix=lasttoken,path=".";
    vector<string> files;
    DIR* dir = opendir(path.c_str());
    if (dir){
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr){
                if (entry->d_name[0] != '.'){
                    files.push_back(entry->d_name);
                }
            }
        closedir(dir);
    }
    vector<string> suggestions;
        for (const auto& file : files){
            if (file.find(lasttoken) == 0) { 
                suggestions.push_back(file);
            }
        }
        if(suggestions.size()==0){
            if(lasttoken[0]>='A' && lasttoken[0]<='Z' || lasttoken[0]>='a' && lasttoken[0]<='z' || lasttoken[0]>='0' && lasttoken[0]<='9' )
            return lasttoken;
            else 
            return " ";
        }
             prefix = suggestions[0];
            if(!suggestions.empty()){
                for (size_t i = 1; i < suggestions.size(); ++i) {
                    int j = 0;
                    while (j < prefix.size() && j < suggestions[i].size() && prefix[j] == suggestions[i][j]) {
                        ++j;
                    }
                     prefix = prefix.substr(0, j);
                    if (prefix.empty()) {
                        break;
                    }
                }
            }
            
        if(prefix==lasttoken && suggestions.size()>1){
            cout<<endl;
            for(auto i:suggestions){
                cout<<i<<" ";
            }
            return "";
        }
        else{
        return prefix;   
        }
        return "";
        
}

string commandsuggestionfun(string line){
    string path="/usr/bin",ans="";
    std::vector<std::string> commands;
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != nullptr) {
            commands.push_back(ent->d_name);
        }
        closedir(dir);
    }

    if(commands.size()==1){
        return commands[0];
    }
    cout<<endl;
    for (const auto& cmd : commands){
        if (cmd.find(line) == 0) { 
            cout<<cmd<<" ";
        }
    }
    return "";
}