#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>


void help(){
    std::cout<<"帮助信息:"<<std::endl;
    std::cout<<"1 搜索              search keyword"<<std::endl;
    std::cout<<"2 帮助              help"<<std::endl;
    std::cout<<"3 播放               play index"<<std::endl;
    std::cout<<"4 退出               exit"<<std::endl;
    std::cout<<"5 设置播放速度        speed times"<<std::endl;
    std::cout<<"6 设置画面清晰度      clarity level "<<std::endl;
    std::cout<<"7 浏览历史           history"<<std::endl;
    std::cout<<"8 收藏夹             starfolder"<<std::endl;
    std::cout<<"9 收藏               star index"<<std::endl;
}
int judge_input(std::string input){
    if(input=="exit"){
        return 0;
    }
    if(input=="help"){
        help();
    }
    if(input=="play"){

    }
    if(input=="search"){

    }
    std::cout<<"错误输入";
    help();
}
int main() {
    std::cout<<"欢迎打开bilibiliCLI"<<std::endl;
    help();
    curl_global_init(CURL_GLOBAL_ALL);

    while(true){

        CURL* curl = curl_easy_init();

        std::string keyword;
        std::cout << "请输入B站搜索关键词：";
        std::getline(std::cin, keyword);
        if(keyword=="exit"){
            std::cout<<"已退出"<<std::endl;
            break;
        }
        std::string encodedKey = urlEncode(curl, keyword);

        std::cout << "正在搜索..." << std::endl;
        std::string jsonResp = fetchBiliAPI(encodedKey);
        std::vector<VideoInfo> videos = parseJson(jsonResp);

        if (videos.empty()) {
            std::cout << "\n❌ 未找到视频！\n";
            return 0;
        }

        std::cout << "\n========== 搜索结果 ==========\n";
        for (size_t i = 0; i < videos.size(); ++i) {
            std::cout << i + 1 << ". " << videos[i].title << "\nURL: " << videos[i].url << "\n\n";
        }

        std::string input;
        std::cout << "请输入序号播放，播放速度，画面清晰度：";
        std::cin >> input;
        if(input=="exit"){
            std::cout<<"已退出"<<std::endl;
            break;
        }

        std::string cmd = "yt-dlp -o - \"" + videos[choice-1].url + "\" | ffplay -i - -autoexit";
        system(cmd.c_str());

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}