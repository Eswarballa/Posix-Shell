#include<iostream>
#include<bits/stdc++.h>
#include <cstdlib> 
#include <fstream>
#include "header.h"
using namespace std;
void writehistory(deque<string> history){
    
    std::ofstream outfile("history.txt");
        if (!outfile) {
            cerr << "Failed to open file: history.txt" << endl;
            return;
        }
        for (auto command : history) {
            outfile << command << endl;
        }
        outfile.close();
        return;
}
void historydisplay(deque<string> history, vector<string> tokens){
    int len=10,qsize=history.size();
    if(tokens.size()>1){
    len=stoi(tokens[1]);
   }
    len=min(len,qsize);   
   for(int i=history.size()-len;i<history.size();i++){
    cout<<history[i]<<endl;
   }
    return;
}
void readhistory(deque<string> &history){
     ifstream infile("history.txt");
        string command;

        if (!infile) {
            cerr << "Failed to open file: history.txt" << endl;
            return;
        }
        history.clear(); 
        while (getline(infile, command)) {
            if(history.size()==20){
                history.pop_front();
            }
            history.push_back(command);
        }

        infile.close();
    return;
}
