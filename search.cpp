#include "search.h"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;

// ── CURL 回调 ────────────────────────────────────────────────────────────────
static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ── CURL 初始化 ───────────────────────────────────────────────────────────────
static CURL* initCurl() {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 300L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    return curl;
}

// ── URL 编码 ──────────────────────────────────────────────────────────────────
static std::string urlEncode(CURL* curl, const std::string& str) {
    char* encoded = curl_easy_escape(curl, str.c_str(), (int)str.length());
    std::string result(encoded);
    curl_free(encoded);
    return result;
}

// ── 去除 HTML 标签（<em>, </em> 等 B 站高亮标记）────────────────────────────
static std::string stripHtmlTags(const std::string& s) {
    std::string out;
    bool inTag = false;
    for (char c : s) {
        if (c == '<') { inTag = true; continue; }
        if (c == '>') { inTag = false; continue; }
        if (!inTag) out += c;
    }
    return out;
}

// ── 请求 B 站搜索 API ─────────────────────────────────────────────────────────
static std::string fetchBiliAPI(const std::string& encodedKeyword) {
    CURL* curl = initCurl();
    std::string response;
    if (!curl) return response;

    std::string url = "https://api.bilibili.com/x/web-interface/search/all/v2?keyword=" + encodedKeyword;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers,
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 Chrome/130.0.0.0 Safari/537.36");
    headers = curl_slist_append(headers, "Referer: https://search.bilibili.com/");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "❌ 网络请求失败: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

// ── 解析 JSON ─────────────────────────────────────────────────────────────────
static std::vector<VideoInfo> parseJson(const std::string& jsonStr) {
    std::vector<VideoInfo> videos;
    if (jsonStr.empty()) return videos;

    try {
        json data = json::parse(jsonStr);
        if (data["code"] != 0) {
            std::cout << "❌ B站 API 错误: " << data["message"].get<std::string>() << std::endl;
            return videos;
        }
        for (auto& result : data["data"]["result"]) {
            if (result["result_type"] == "video") {
                for (auto& item : result["data"]) {
                    VideoInfo info;
                    info.title = stripHtmlTags(item["title"].get<std::string>());
                    info.url   = "https://www.bilibili.com/video/" + item["bvid"].get<std::string>();
                    videos.push_back(info);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "❌ JSON 解析失败: " << e.what() << std::endl;
    }
    return videos;
}

// ── 对外接口 ──────────────────────────────────────────────────────────────────
std::vector<VideoInfo> search(const std::string& keyword) {
    CURL* curl = initCurl();
    std::string encoded = urlEncode(curl, keyword);
    curl_easy_cleanup(curl);

    std::cout << "🔍 正在搜索「" << keyword << "」..." << std::endl;
    std::string jsonResp = fetchBiliAPI(encoded);
    std::vector<VideoInfo> videos = parseJson(jsonResp);

    if (videos.empty()) {
        std::cout << "❌ 未找到相关视频" << std::endl;
        return videos;
    }

    std::cout << "\n========== 搜索结果 ==========\n";
    for (size_t i = 0; i < videos.size(); ++i) {
        std::cout << i + 1 << ". " << videos[i].title << "\n   " << videos[i].url << "\n\n";
    }
    return videos;
}
