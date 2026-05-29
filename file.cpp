#include "file.h"
#include <iostream>
#include <fstream>
#include <sstream>

// 格式：每行 "标题\tURL"（用 tab 分隔，标题里不含 tab）
void writeRecord(const VideoInfo& v, const std::string& filename) {
    // 用追加模式，不会覆盖旧记录
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile) {
        std::cout << "❌ 文件写入失败: " << filename << std::endl;
        return;
    }
    outFile << v.title << "\t" << v.url << "\n";
    outFile.close();
}

std::vector<VideoInfo> readRecords(const std::string& filename) {
    std::vector<VideoInfo> records;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cout << "（暂无记录）" << std::endl;
        return records;
    }
    std::string line;
    int idx = 1;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        size_t tab = line.find('\t');
        if (tab == std::string::npos) continue; // 格式不对的行跳过
        VideoInfo v;
        v.title = line.substr(0, tab);
        v.url   = line.substr(tab + 1);
        records.push_back(v);
        std::cout << idx++ << ". " << v.title << "\n   " << v.url << "\n\n";
    }
    inFile.close();
    return records;
}

void deleteRecord(const std::string& filename, int index) {
    // index 从 1 开始
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cout << "❌ 文件不存在: " << filename << std::endl;
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    inFile.close();

    if (index < 1 || index > (int)lines.size()) {
        std::cout << "❌ 编号超出范围" << std::endl;
        return;
    }
    lines.erase(lines.begin() + index - 1);

    std::ofstream outFile(filename, std::ios::trunc);
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
    std::cout << "✅ 已删除" << std::endl;
}
