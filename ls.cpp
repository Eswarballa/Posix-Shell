#include<bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <filesystem>
#include <time.h>
#include "header.h"
using namespace std;
//dirs  .. . ~ <dirname>    
//flags  -la -al -l -a
void filelsprint(string fullpath ,string flag){
    struct stat filestat;
    std::filesystem::path path(fullpath);
    string filename=path.filename().string();
    if (stat(fullpath.c_str(), &filestat) == -1) {
            cout<< "ls: cannot access "<<filename<<" No such file or directory"<< endl;
    }
    if(flag=="ls" || flag=="-a"){
        cout<<filename;
        return;
    }else if(flag=="-la" || flag=="-al"){
            cout << (S_ISDIR(filestat.st_mode) ? 'd' : '-')<< ((filestat.st_mode & S_IRUSR) ? 'r' : '-')<< ((filestat.st_mode & S_IWUSR) ? 'w' : '-')<< ((filestat.st_mode & S_IXUSR) ? 'x' : '-')<< ((filestat.st_mode & S_IRGRP) ? 'r' : '-')
            <<((filestat.st_mode & S_IWGRP) ? 'w' : '-')<< ((filestat.st_mode & S_IXGRP) ? 'x' : '-')<< ((filestat.st_mode & S_IROTH) ? 'r' : '-')<< ((filestat.st_mode & S_IWOTH) ? 'w' : '-')<< ((filestat.st_mode & S_IXOTH) ? 'x' : '-');
            cout << " " << filestat.st_nlink;

            struct passwd* pw = getpwuid(filestat.st_uid);
            struct group* gr = getgrgid(filestat.st_gid);
            cout << " " << (pw ? pw->pw_name : "unknown")<< " " << (gr ? gr->gr_name : "unknown");
            cout << " " << setw(6) << filestat.st_size;
                

            char timeBuf[80];
            struct tm* tm_info = localtime(&filestat.st_mtime);
            strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", tm_info);
            cout << " " << timeBuf;
            cout << " " << filename<<endl;
    }else if(flag=="-l"){
            cout<< (S_ISDIR(filestat.st_mode) ? 'd' : '-')<< ((filestat.st_mode & S_IRUSR) ? 'r' : '-')<< ((filestat.st_mode & S_IWUSR) ? 'w' : '-')<< ((filestat.st_mode & S_IXUSR) ? 'x' : '-')<< ((filestat.st_mode & S_IRGRP) ? 'r' : '-')
            <<((filestat.st_mode & S_IWGRP) ? 'w' : '-')<< ((filestat.st_mode & S_IXGRP) ? 'x' : '-')<< ((filestat.st_mode & S_IROTH) ? 'r' : '-')<< ((filestat.st_mode & S_IWOTH) ? 'w' : '-')<< ((filestat.st_mode & S_IXOTH) ? 'x' : '-');
            cout << " " << filestat.st_nlink;

            struct passwd* pw = getpwuid(filestat.st_uid);
            struct group* gr = getgrgid(filestat.st_gid);
            cout << " " << (pw ? pw->pw_name : "unknown")<< " " << (gr ? gr->gr_name : "unknown");
            cout << " " << setw(8) << filestat.st_size;
            char timeBuf[80];
            struct tm* tm_info = localtime(&filestat.st_mtime);
            strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", tm_info);
            cout << " " << timeBuf;
            cout << " " << filename<< std::endl;
    }else{
         cout<<"ls: cannot access "<<filename<<" No such file or directory"<<endl;
    }
    cout<<endl;
    return;
}


bool isFile(string &path){
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        cout<< "ls: cannot access : No such file or directory" << endl;
        return false;
    }
    return (info.st_mode & S_IFREG) != 0;
}
void printls(vector<string> tokens){
    char oldpath[100];
        if(getcwd(oldpath,100)==NULL){
            cout<<"ls: cannot access '"<<tokens[0]<<"': No such file or directory";
            return;
        }
    char prepath[100];
    if(tokens[0]=="."){
                if(getcwd(prepath,100)==NULL){
                    return;
                }
    }else if(tokens[0]==".."){
                if (chdir("..") != 0) {
                    return ;
                }
                
                if(getcwd(prepath,100)==NULL){
                    return;
                }
                chdir(oldpath);
    }else if(tokens[0]=="~"){
        const char* homeDir = getenv("HOME");
                if (homeDir != nullptr) {
                    if(chdir(homeDir)==0){
                        if(getcwd(prepath,100)==NULL){
                            return;
                        }
                    }
                }
                chdir(oldpath);
    }else if(tokens[0][0]>='a' && tokens[0][0]<='z' || tokens[0][0]>'A' && tokens[0][0]<'Z'){
            string fullPath = string(oldpath) + "/" + tokens[0];
            if(isFile(fullPath)){
                filelsprint(fullPath,tokens[1]);
                return;
            }

            if(chdir(fullPath.c_str())!=0){
                cout<<"ls: cannot access '"<<tokens[0]<<"': No such file or directory"<<endl;
                return;
            }
            if(getcwd(prepath,100)==NULL){
                    return;
                }
            chdir(oldpath);
            }else{
            cout<<"ls: cannot access "<<tokens[1]<<": No such file or directory"<<endl;
            }

        DIR* dir = opendir(prepath);
        if (dir == nullptr) {
            cout << "ls: cannot access '" << tokens[0] << "': No such file or directory" << endl;
            return;
        }
        struct dirent* entry;
        if(tokens[1]=="ls" || tokens[1]=="-a"){
            while ((entry = readdir(dir)) != nullptr){
                if (entry->d_name[0] != '.'){
                    cout << entry->d_name << " ";
                }
            }
            cout<<endl;
        }else if(tokens[1]=="-al" || tokens[1]=="-la"){
             off_t totalBlocks = 0;
                struct stat filestat;
                while ((entry = readdir(dir)) != nullptr) {
                    string fullPath = string(prepath) + "/" + entry->d_name;
                    if (stat(fullPath.c_str(), &filestat) == -1) {
                        cout<< "ls: cannot access "<< entry->d_name <<": No such file or directory" <<endl;
                        continue;
                    }
                    totalBlocks += filestat.st_blocks;
                }
                cout << "total " << totalBlocks / 2 <<endl; 
                rewinddir(dir);
                while ((entry = readdir(dir)) != nullptr){
                    struct stat filestat;
                    string filepath=string(prepath)+"/"+entry->d_name;
                    if(stat(filepath.c_str(),&filestat)==-1){
                        cout<<"ls: cannot access "<< entry->d_name <<": No such file or directory"<< endl;
                        continue;
                    }
                    cout << (S_ISDIR(filestat.st_mode) ? 'd' : '-')<< ((filestat.st_mode & S_IRUSR) ? 'r' : '-')<< ((filestat.st_mode & S_IWUSR) ? 'w' : '-')<< ((filestat.st_mode & S_IXUSR) ? 'x' : '-')<< ((filestat.st_mode & S_IRGRP) ? 'r' : '-')
                    <<((filestat.st_mode & S_IWGRP) ? 'w' : '-')<< ((filestat.st_mode & S_IXGRP) ? 'x' : '-')<< ((filestat.st_mode & S_IROTH) ? 'r' : '-')<< ((filestat.st_mode & S_IWOTH) ? 'w' : '-')<< ((filestat.st_mode & S_IXOTH) ? 'x' : '-');
                    cout << " " << filestat.st_nlink;

                    struct passwd* pw = getpwuid(filestat.st_uid);
                    struct group* gr = getgrgid(filestat.st_gid);
                    cout << " " << (pw ? pw->pw_name : "unknown")<< " " << (gr ? gr->gr_name : "unknown");
                    cout << " " << setw(6) << filestat.st_size;
                

                    char timeBuf[80];
                    struct tm* tm_info = localtime(&filestat.st_mtime);
                    strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", tm_info);
                    cout << " " << timeBuf;
                    cout << " " << entry->d_name<<endl;

                }
        }else if(tokens[1]=="-l"){
                off_t totalBlocks = 0;
                struct stat filestat;
                while ((entry = readdir(dir)) != nullptr) {
                    string fullPath = string(prepath) + "/" + entry->d_name;
                    if (stat(fullPath.c_str(), &filestat) == -1) {
                        cout<< "ls: cannot access "<<tokens[1]<<": No such file or directory" << entry->d_name << std::endl;
                        continue;
                    }
                    totalBlocks += filestat.st_blocks;
                }
                cout << "total " << totalBlocks / 2 << std::endl; 
                rewinddir(dir);
                while ((entry = readdir(dir)) != nullptr ){
                    if (entry->d_name[0] != '.'){
                        string filepath=string(prepath)+"/"+entry->d_name;
                        if(stat(filepath.c_str(),&filestat)==-1){
                            cout<<"ls: cannot access "<<tokens[1]<<": No such file or directory" << entry->d_name << std::endl;
                            continue;
                        }
                        cout<< (S_ISDIR(filestat.st_mode) ? 'd' : '-')<< ((filestat.st_mode & S_IRUSR) ? 'r' : '-')<< ((filestat.st_mode & S_IWUSR) ? 'w' : '-')<< ((filestat.st_mode & S_IXUSR) ? 'x' : '-')<< ((filestat.st_mode & S_IRGRP) ? 'r' : '-')
                        <<((filestat.st_mode & S_IWGRP) ? 'w' : '-')<< ((filestat.st_mode & S_IXGRP) ? 'x' : '-')<< ((filestat.st_mode & S_IROTH) ? 'r' : '-')<< ((filestat.st_mode & S_IWOTH) ? 'w' : '-')<< ((filestat.st_mode & S_IXOTH) ? 'x' : '-');
                        cout << " " << filestat.st_nlink;

                        struct passwd* pw = getpwuid(filestat.st_uid);
                        struct group* gr = getgrgid(filestat.st_gid);
                        cout << " " << (pw ? pw->pw_name : "unknown")<< " " << (gr ? gr->gr_name : "unknown");
                        cout << " " << setw(8) << filestat.st_size;

            

                        char timeBuf[80];
                        struct tm* tm_info = localtime(&filestat.st_mtime);
                        strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", tm_info);
                        cout << " " << timeBuf;
                        cout << " " << entry->d_name << std::endl;
                    }
                }
        }else{
                cout<<"ls: cannot access "<<tokens[1]<<": No such file or directory"<<endl;
        }
}

void lsfun(vector<string> tokens){
        vector<string> dirs,flags;
        string final_flag="ls";
        for(int i=1;i<tokens.size();i++){
            if(tokens[i][0]=='-'){
                flags.push_back(tokens[i]);
            }else{
                dirs.push_back(tokens[i]);
            }
        }
        if((find(flags.begin(), flags.end(), "-al") != flags.end()) || 
       (find(flags.begin(), flags.end(), "-la") != flags.end())){
        final_flag = "-al";
        }
        else if(find(flags.begin(), flags.end(), "-l") != flags.end()){
            final_flag = "-l";
        }
        else if(find(flags.begin(), flags.end(), "-a") != flags.end()){
            final_flag = "-a";
        }
        else{
            final_flag = "ls";
        }
        vector<string> tokens2;
        if(dirs.size()==0){
            dirs.push_back(".");
        }
        
        for(auto i: dirs){
            tokens2={i,final_flag};
            if(dirs.size()!=1)
            cout<<i<<":"<<endl;
            printls(tokens2);
        }

    
}