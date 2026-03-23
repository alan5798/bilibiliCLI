#include <iostream>
#include <sstream>
#include "help.h"
#include "input.h"
int cmdconfirm(){
    std::string cmd;
    std::cout<<">";
    std::cin>>cmd;
    std::string cmdlist[9]={"search","help","play","exit","speed","clarity","history","starfolder","star"};
    for(int i=0;i<9;++i){
        if(cmd==cmdlist[i]){
            return i;
        }
    }
    std::cout<<"wrong input";
    help();
    return -1;
}

