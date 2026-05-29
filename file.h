#pragma once
#include <string>
#include <vector>
#include "video.h"

// 追加写入一条视频记录到文件
void writeRecord(const VideoInfo& v, const std::string& filename);

// 读取文件并显示带编号的列表，返回所有记录（供直接播放用）
std::vector<VideoInfo> readRecords(const std::string& filename);

// 从收藏/历史记录中删除指定编号的条目
void deleteRecord(const std::string& filename, int index);
