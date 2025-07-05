#include<iostream>
#include "header.h"
using namespace std;

void echofun(vector<string> tokens){
    string str="",command;
    for(int i=1;i<tokens.size();i++){
            str+=tokens[i]+" ";     
    }
    cout<<str<<endl;
    return ;
}