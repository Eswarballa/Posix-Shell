#include<iostream>
#include <bits/stdc++.h> 
#include <cstdlib> 
#include<unistd.h>
#include "header.h"
using namespace std;
void userinpfun(string commandline,deque<string> &history){
    if(commandline=="")
    return;
        char path[100];
            if(getcwd(path,100)==NULL){
                cerr<<"Error getting current working directory path"<<errno;
            }
        string currpath="";
        vector<string> tokens;
        stringstream commands(commandline);
        string token;
        while(commands >> token){
            tokens.push_back(token);
        }
        if(find(tokens.begin(),tokens.end(),"|")!=tokens.end()){
            pipefun(tokens);
   
             return;
        }
        if(find(tokens.begin(),tokens.end(),">")!=tokens.end() || find(tokens.begin(),tokens.end(),"<")!=tokens.end() || find(tokens.begin(),tokens.end(),">>")!=tokens.end() ){
            redirectionfun(tokens,STDIN_FILENO,STDOUT_FILENO);
            return;
        }
        if(tokens.size()!=0){

            if(commandline==""){
                    return ;
            }else if(tokens[0]=="echo"){
                echofun(tokens);
            }
            else if(tokens[0]=="pwd"){
                cout<<path<<endl;
            }
            else if(tokens[0]=="clear"){
                if(tokens.size()==1)
                foregroundfun(tokens);
            }
            else if(tokens[0]=="ls"){
                lsfun(tokens);
            }
            else if(tokens[0]=="cd"){
                cdfun(tokens);
                return;
            }else if(tokens[0]=="search"){
                bool sec=false;
                if(tokens.size()!=1){
                    sec=searchfun(tokens);
                }
                else{
                    cout<<"Enter file name"<<endl;
                }
                if(sec){
                    cout<<"True"<<endl;
                }
                else{
                    cout<<"False"<<endl;
                }
            }else if(tokens[0]=="history"){
                writehistory(history);
                readhistory(history);
                historydisplay(history,tokens);
            }
            else if(tokens[tokens.size()-1]=="&"){
                backgroundfun(tokens);
            }else if(tokens[0]=="pinfo"){
                pinfofun(tokens);
            }
            else{
                foregroundfun(tokens);
             }
        }

        return ;
}