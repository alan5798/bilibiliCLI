#include <iostream>
#include <vector>
#include <cstdlib>
#include "search.h"
#include "play.h"
#include "help.h"
#include "input.h"

int main() {
    std::cout<<"欢迎打开bilibiliCLI"<<std::endl;
    help();
    bool terminate=false;
    while(terminate==false){
        int cmd=cmdconfirm();
        std::string argu;
        switch(cmd){
            case 0:
            std::cin>>argu;
            search(argu);
            break;
            case 1:
            std::cout<<"感谢使用bilibiliCLI"<<std::endl;
            terminate=true;
            case 2:
            std::cin>>argu;

            default:

        }
        
        
        
    }
    return 0;
}