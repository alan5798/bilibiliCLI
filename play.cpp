#include <iostream>
#include <vector>
#include <fstream>
#include "video.h"
#include "file.h"
void play(std::vector<struct VideoInfo> videos,int speed,int clarity,int choice){
    // 修复：ffplay 正确管道播放 + 自动退出 + 不报错
    std::string url = videos[choice-1].url;
    std::string cmd = "yt-dlp -o - \"" + url + "\" | ffplay -i - -autoexit";
    write(videos[choice-1],"history.txt");
    system(cmd.c_str());
}