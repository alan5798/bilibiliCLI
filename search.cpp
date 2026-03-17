#include "search.h"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;
// CURL回调
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

// URL编码
static std::string urlEncode(CURL* curl, const std::string& str) {
    char* encoded = curl_easy_escape(curl, str.c_str(), str.length());
    std::string result(encoded);
    curl_free(encoded);
    return result;
}

// 请求API
static std::string fetchBiliAPI(const std::string& keyword) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string url = "https://api.bilibili.com/x/web-interface/search/all/v2?keyword=" + keyword;
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 Chrome/130.0.0.0 Safari/537.36");
        headers = curl_slist_append(headers, "Referer: https://search.bilibili.com/");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) printf("CURL请求失败: %s\n", curl_easy_strerror(res));

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return response;
}

// 调试：打印字符串片段
static void debugPrint(const std::string& s, const char* name, int maxLen=1000){
    printf("\n[调试] %s 长度：%zu\n", name, s.size());
    printf("[调试] %s 前%d字符：\n", name, maxLen);
    if(s.size() < maxLen) printf("%s\n", s.c_str());
    else printf("%s...\n", s.substr(0, maxLen).c_str());
}

// 解析JSON（带调试+异常捕获）
static std::vector<VideoInfo> parseJson(const std::string& jsonStr) {
    std::vector<VideoInfo> videos;
    try {
        debugPrint(jsonStr, "API返回JSON");
        json data = json::parse(jsonStr);

        // 调试：打印返回码
        printf("[调试] B站API返回码 code: %d\n", data["code"].get<int>());
        if(data["code"] != 0){
            printf("[调试] API错误信息: %s\n", data["message"].get<std::string>().c_str());
            return videos;
        }

        // 遍历所有结果，找视频
        for(auto& result : data["data"]["result"]){
            printf("[调试] 结果类型: %s\n", result["result_type"].get<std::string>().c_str());
            if(result["result_type"] == "video"){
                for(auto& item : result["data"]){
                    VideoInfo info;
                    info.title = item["title"].get<std::string>();
                    size_t pos;
                    while((pos=info.title.find('<'))!=std::string::npos) info.title.erase(pos,1);
                    while((pos=info.title.find('>'))!=std::string::npos) info.title.erase(pos,1);
                    info.url = "https://www.bilibili.com/video/" + item["bvid"].get<std::string>();
                    videos.push_back(info);
                }
            }
        }
    } catch (const std::exception& e) {
        printf("[调试] 解析异常: %s\n", e.what());
    }
    printf("[调试] 解析到视频数量: %zu\n", videos.size());
    return videos;
}
std::vector<struct VideoInfo> search(std::string keyword){
    CURL* curl = curl_easy_init();
    std::string encodedKey = urlEncode(curl, keyword);

    std::cout << "正在搜索..." << std::endl;
    std::string jsonResp = fetchBiliAPI(encodedKey);
    std::vector<VideoInfo> videos = parseJson(jsonResp);

    if (videos.empty()) {
        std::cout << "\n❌ 未找到视频！\n";
        //process wrong information
    }

    std::cout << "\n========== 搜索结果 ==========\n";
    for (size_t i = 0; i < videos.size(); ++i) {
        std::cout << i + 1 << ". " << videos[i].title << "\nURL: " << videos[i].url << "\n\n";
    }
    return videos;
}