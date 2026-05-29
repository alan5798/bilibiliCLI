#include <iostream>
#include <vector>
#include <string>
#include "search.h"
#include "play.h"
#include "help.h"
#include "input.h"
#include "file.h"
#include "video.h"

static const std::string HISTORY_FILE = "history.txt";
static const std::string STAR_FILE    = "star.txt";

// 把字符串转为 int，失败返回 -1
static int toInt(const std::string& s) {
    if (s.empty()) return -1;
    try { return std::stoi(s); }
    catch (...) { return -1; }
}

// 从 vector 里按编号（1-based）取视频，越界返回 nullptr
static const VideoInfo* pickVideo(const std::vector<VideoInfo>& list, int idx) {
    if (idx < 1 || idx > (int)list.size()) {
        std::cout << "❌ 编号超出范围（共 " << list.size() << " 条）" << std::endl;
        return nullptr;
    }
    return &list[idx - 1];
}

int main() {
    std::cout << "\n欢迎使用 bilibiliCLI 🎬\n";
    help();

    std::vector<VideoInfo> searchResult;   // 当前搜索结果
    std::string clarity = "best";          // 当前画质设置
    double speed = 1.0;                    // 当前倍速设置

    while (true) {
        std::string cmd, arg;
        getCommand(cmd, arg);

        // ── 退出 ──────────────────────────────────────────────────────────────
        if (cmd == "q" || cmd == "quit" || cmd == "exit") {
            std::cout << "感谢使用 bilibiliCLI，再见！👋" << std::endl;
            break;
        }

        // ── 帮助 ──────────────────────────────────────────────────────────────
        else if (cmd == "h" || cmd == "help") {
            help();
        }

        // ── 搜索 ──────────────────────────────────────────────────────────────
        else if (cmd == "s" || cmd == "search") {
            if (arg.empty()) {
                std::cout << "用法：s <关键词>" << std::endl;
            } else {
                searchResult = search(arg);
            }
        }

        // ── 播放搜索结果 ──────────────────────────────────────────────────────
        else if (cmd == "p" || cmd == "play") {
            if (searchResult.empty()) {
                std::cout << "❌ 没有搜索结果，请先用 s <关键词> 搜索" << std::endl;
                continue;
            }
            int idx = toInt(arg);
            const VideoInfo* v = pickVideo(searchResult, idx);
            if (v) {
                play(v->url, clarity, speed);
                writeRecord(*v, HISTORY_FILE); // 播放后写入历史
            }
        }

        // ── 画质设置 ──────────────────────────────────────────────────────────
        else if (cmd == "res") {
            if (arg == "360" || arg == "480" || arg == "720" || arg == "1080" || arg == "best") {
                clarity = arg;
                std::cout << "✅ 画质设置为: " << clarity << std::endl;
            } else {
                std::cout << "用法：res <360 / 480 / 720 / 1080 / best>" << std::endl;
            }
        }

        // ── 倍速设置 ──────────────────────────────────────────────────────────
        else if (cmd == "speed") {
            try {
                double s = std::stod(arg);
                if (s <= 0 || s > 4.0) throw std::out_of_range("");
                speed = s;
                std::cout << "✅ 播放速度设置为: x" << speed << std::endl;
            } catch (...) {
                std::cout << "用法：speed <0.5 / 1.0 / 1.5 / 2.0>" << std::endl;
            }
        }

        // ── 历史记录：查看 ────────────────────────────────────────────────────
        else if (cmd == "history") {
            std::cout << "\n========== 历史记录 ==========\n";
            readRecords(HISTORY_FILE);
        }

        // ── 历史记录：播放 ────────────────────────────────────────────────────
        else if (cmd == "hp") {
            std::vector<VideoInfo> history = readRecords(HISTORY_FILE);
            // readRecords 会打印列表，但这里静默调用一次再取
            // 实际上我们要先展示后播，先读一次
            if (history.empty()) {
                std::cout << "❌ 历史记录为空" << std::endl;
                continue;
            }
            // 如果 arg 空，重新显示并让用户输入
            int idx = toInt(arg);
            const VideoInfo* v = pickVideo(history, idx);
            if (v) {
                play(v->url, clarity, speed);
                writeRecord(*v, HISTORY_FILE);
            }
        }

        // ── 历史记录：删除 ────────────────────────────────────────────────────
        else if (cmd == "hd") {
            int idx = toInt(arg);
            if (idx == -1) { std::cout << "用法：hd <编号>" << std::endl; continue; }
            deleteRecord(HISTORY_FILE, idx);
        }

        // ── 收藏夹：查看 ──────────────────────────────────────────────────────
        else if (cmd == "star") {
            std::cout << "\n========== 收藏夹 ==========\n";
            readRecords(STAR_FILE);
        }

        // ── 收藏夹：收藏搜索结果 ──────────────────────────────────────────────
        else if (cmd == "ss") {
            if (searchResult.empty()) {
                std::cout << "❌ 没有搜索结果，请先搜索" << std::endl;
                continue;
            }
            int idx = toInt(arg);
            const VideoInfo* v = pickVideo(searchResult, idx);
            if (v) {
                writeRecord(*v, STAR_FILE);
                std::cout << "⭐ 已收藏：" << v->title << std::endl;
            }
        }

        // ── 收藏夹：播放 ──────────────────────────────────────────────────────
        else if (cmd == "sp") {
            std::cout << "\n========== 收藏夹 ==========\n";
            std::vector<VideoInfo> stars = readRecords(STAR_FILE);
            if (stars.empty()) {
                std::cout << "❌ 收藏夹为空" << std::endl;
                continue;
            }
            int idx = toInt(arg);
            const VideoInfo* v = pickVideo(stars, idx);
            if (v) {
                play(v->url, clarity, speed);
                writeRecord(*v, HISTORY_FILE);
            }
        }

        // ── 收藏夹：删除 ──────────────────────────────────────────────────────
        else if (cmd == "sd") {
            int idx = toInt(arg);
            if (idx == -1) { std::cout << "用法：sd <编号>" << std::endl; continue; }
            deleteRecord(STAR_FILE, idx);
        }

        // ── 未知命令 ──────────────────────────────────────────────────────────
        else if (!cmd.empty()) {
            std::cout << "❓ 未知命令「" << cmd << "」，输入 h 查看帮助" << std::endl;
        }
    }

    return 0;
}
