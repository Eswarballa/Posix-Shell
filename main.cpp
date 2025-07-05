#include<iostream>
#include <bits/stdc++.h> 
#include <sstream>
#include <termios.h>
#include "header.h"
using namespace std;
void handle_sig(int sig){
    
}
int main(){
    string commandline;
    deque<string> history;
    char hstname[50];
    readhistory(history);
    if(gethostname(hstname,50)==-1){
        cout<<"Error in getting hostname"<<errno;
        return 1;
    }
    char path[100];
    if(getcwd(path,100)==NULL){
        cerr<<"Error getting current working directory path"<<errno;
        return 1;
    }
    string prev_path=string(path),line; 
    int prev_path_len=prev_path.length();
    const char* username=getenv("USER");
    signal(SIGINT, handle_sig);
    if(username==nullptr){
        cout<<"error in getting username";
        return 1;
    }
    else{
        system("clear");
    }
    string attherate="@",str,temp;
    string currpath="";
    str=username+attherate+hstname+":~";
    while(true){

        cout<<"\033[32m"<<str<<"\033[0m";
        cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
        
        
        struct termios original_tty;
        tcgetattr(STDIN_FILENO, &original_tty);

        struct termios raw_tty = original_tty;
        raw_tty.c_lflag &= ~(ICANON | ECHO);
        raw_tty.c_cc[VMIN] = 1;
        raw_tty.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &raw_tty);
         char ch;
         int k=history.size(),tab=0;
         line="";
        while (true){
            ch = getchar();
            if(ch == '\n') {  
                break;
            } else if (ch == 127){  //backspace
                if (!line.empty()) {
                    line.pop_back();   
                }
                cout << "\033[2K\r";
                cout<<"\033[32m"<<str<<"\033[0m";
                cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                cout<<line;     
            }else if(ch == '\t'){ //tabspace
                if(line!=""){
                    int pos=line.find(' ');            
                    if(pos != string::npos && pos!=line.length()){
                        string s=filesuggestionfun(line);
                        if(s==""){
                            tab=1;
                            break;
                        }
                        pos = line.find_last_of(' ');
                        line = line.substr(0,pos + 1) + s;
                        //trim line here
                        
                        cout << "\033[2K\r";
                        cout<<"\033[32m"<<str<<"\033[0m";
                        cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                        cout<<line;
                    }else{
                        string s=commandsuggestionfun(line);
                        if(s!=""){
                            line="";
                            break;  
                        }else{
                            line="";
                            break;
                        }                           
                    }
                }
            }else if(ch == '\033'){ //arrow
                char seq[2];
                seq[0] = getchar();
                seq[1] = getchar();
                if (seq[0] == '[') {
                    if (seq[1] == 'A') {
                        if(k>0){
                            k--;
                        }
                        cout << "\033[2K\r";
                        cout<<"\033[32m"<<str<<"\033[0m";
                        cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                        line=history[k];
                        cout<<line;
                    }else if(seq[1]=='B'){
                        if(k<history.size()){
                            k++;
                        }
                    cout << "\033[2K\r";
                    cout<<"\033[32m"<<str<<"\033[0m";
                    cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                    line=history[k];
                    cout<<line;                                      
                 }
                }
            }else if(ch == '\x1A'){  //  Ctrl+Z
                    signal(SIGTSTP, SIG_DFL); 
                    raise(SIGTSTP); 
            } else if (ch == '\x03'){  //  Ctrl+C
                                        
            } else if (ch == '\x04'){  //  Ctrl+D
                   cout<<endl;
                   exit(0);
            }else {
                line += ch; 
                cout << ch;
            }
        
        }
        cout<<endl;
        tcsetattr(STDIN_FILENO, TCSANOW, &original_tty);

        commandline=string(line);
        if(commandline.length()!=0 && commandline!=history[history.size()-1]){
            if(history.size()==20){
                history.pop_front();
            }
            history.push_back(commandline);
        }
        vector<string> tokens;
        stringstream commands(commandline);
        string token;
        while(commands >> token){
            tokens.push_back(token);
        }
        if(tokens[0]=="exit"){
            writehistory(history);
            return 1;
        }
        tokens.clear();
        //vector<string> tokens;
        stringstream ss(commandline);

        while (getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        if(tab!=1)
        for(auto i:tokens){
            if(i=="exit" ){
                return 1;
            }
            userinpfun(i,history);
        }
        
        char path[100];
            if(getcwd(path,100)==NULL){
                cerr<<"Error getting current working directory path"<<errno;
            }
         currpath=string(path);
        if(currpath.length()>=prev_path_len){
            currpath=currpath.substr(prev_path_len,currpath.length()-prev_path_len);
        }
    }
    return 0;
}

