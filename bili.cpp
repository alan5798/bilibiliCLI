#include <iostream>
#include <vector>
#include <cstdlib>
#include "search.h"
#include "play.h"
#include "help.h"
#include "input.h"
#include "file.h"
#include <string>
int main() {
    std::cout<<"欢迎打开bilibiliCLI"<<std::endl;
    help();
    bool terminate=false;
    std::vector<struct VideoInfo> sr={};//sr stands for search result
    //initial setting
    int speed=1;
    int clarity=1;
    while(terminate==false){
        int cmd=cmdconfirm();
        std::string argu;
        switch(cmd){
            case 0://search
            std::cout<<">";
            std::cin>>argu;
            sr=search(argu);
            break;//help
            case 1:
            help();
            break;
            case 2://play
            if(sr.size()!=0){
                std::cin>>argu;
                std::string url=sr[stoi(argu)].url;
                play(url);
                write(sr[stoi(argu)],"history.txt");
            }
            break;
            case 3://exit
            std::cout<<"感谢使用bilibiliCLI"<<std::endl;
            terminate=true;
            break;
            case 4:
            std::cout<<">";
            std::cin>>speed;
            break;
            case 5:
            std::cout<<">";
            std::cin>>clarity;
            case 6:
            read("history.txt"); 
            break;
            case 7:
            read("star.txt");
            break;
            case 8:
            if(sr.size()!=0){
                std::cin>>argu;
                write(sr[std::stoi(argu)-1],"star.txt");
            }
            break;
           default:
            break;

        }
    }
    return 0;
}