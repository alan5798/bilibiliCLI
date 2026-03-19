#include "history.h"
void Hwrite(struct VideoInfo v){
    std::ofstream outFile("history.txt");
    if(!outFile){
        std::cout<<"文件打开失败";
        return;
    }
    std::string info=v.title+" "+v.url;
    outFile << info<<std::endl;
    outFile.close();
}
void Hread(){
    std::ifstream inFile("history.txt");
    if(!inFile){
        std::cout<<"文件打开失败";
        return ;
    }
    std::string line;
    while(getline(inFile,line)){
        std::cout<<"line";
    }
    inFile.close();
}