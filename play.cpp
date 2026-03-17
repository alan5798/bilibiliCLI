#include <iostream>
#include <vector>

#include "video.h"
void play(std::vector<struct VideoInfo> videos,int speed,int clarity,int choice){
    std::string cmd = "yt-dlp -o - \"" + videos[choice-1].url + "\" | ffplay -i -speed"+std::to_string(speed) +"lowres"+std::to_string(clarity)+"-autoexit";
    system(cmd.c_str());
}