#ifndef HEADER_H
#define HEADER_H
#include<bits/stdc++.h>
using namespace std;
extern pid_t foreground_pid;
void userinpfun(string commandline,deque<string> &history);
void echofun(std::vector<std::string> tokens);
void cdfun(std::vector<std::string> tokens);
void filelsprint(string fullpath ,string flag);
bool isFile(string &path);
void lsfun(std::vector<std::string> tokens);
void printls(std::vector<std::string> tokens);
void backgroundfun(std::vector<std::string> tokens);
void foregroundfun(std::vector<std::string> tokens);
void pinfofun(std::vector<std::string> tokens);
bool searchfun(std::vector<std::string> tokens);
bool searchfun2(std::string path,std::string filename);
void readhistory(deque<string> &history);
void historydisplay(deque<string> history,vector<string> tokens);
void writehistory(deque<string> history);
void pipefun(vector<string> tokens);
void pipefun2(int n, vector<vector<string>> pipecommands, int input_fd);
void redirectionfun(vector<string> tokens, int input_fd , int output_fd );
string commandsuggestionfun(string line);
string filesuggestionfun(string &line);
void redirectionfun2(vector<string> command, string input_file, string output_file, string iosymbol, int input_fd, int output_fd);
#endif