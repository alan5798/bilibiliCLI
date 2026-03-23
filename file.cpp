#include "file.h"
void write(struct VideoInfo v,std::string filename){
    std::ofstream outFile(filename);
    if(!outFile){
        std::cout<<"文件打开失败";
        return;
    }
    std::string info=v.title+" "+v.url;
    outFile << info<<std::endl;
    outFile.close();
}
void read(std::string filename){
    std::ifstream inFile(filename);
    if(!inFile){
        std::cout<<"文件打开失败";
        return ;
    }
    std::string line;
    while(getline(inFile,line)){
        std::cout<<line<<std::endl;
    }
    inFile.close();
}
