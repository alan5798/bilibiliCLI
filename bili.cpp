#include <iostream>
#include <vector>
#include <cstdlib>
#include "search.h"
#include "play.h"
#include "help.h"
#include "input.h"
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
            std::cin>>argu;
            sr=search(argu);
            break;//help
            case 1:
            help();
            break;
            case 2://play
            if(sr.size()!=0){
                std::cin>>argu;
                play(sr,speed,clarity,std::stoi(argu));
            }
            case 3://exit
            std::cout<<"感谢使用bilibiliCLI"<<std::endl;
            terminate=true;
            break;
            case 4:
            std::cin>>speed;
            break;
            case 5:
            std::cin>>clarity;
            case 6:
            system("cat history.txt");
            break;
            case 7:
            system("cat starfolder.txt");
            case 8:
            if(sr.size()!=0){
                std::cin>>argu;
                std::string cmd="echo ";
            }
            case 9:
            if(sr.size()!=0){
                std::cin>>argu;
                std::string cmd="echo ";
            }
            default:
            break;

        }
    }
    return 0;
}