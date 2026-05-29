#include "play.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

void play(const std::string& url, const std::string& clarity, double speed) {
    // 构建 yt-dlp 的画质选择字符串
    // mpv 内置支持 --ytdl-format，会调用系统里安装的 yt-dlp
    std::string formatStr;
    if (clarity == "best") {
        formatStr = "bestvideo+bestaudio/best";
    } else {
        // 优先选不超过指定分辨率的最佳画质，找不到则降级
        formatStr = "bestvideo[height<=" + clarity + "]+bestaudio/best[height<=" + clarity + "]/best";
    }

    // 把 double speed 转成字符串
    char speedBuf[16];
    snprintf(speedBuf, sizeof(speedBuf), "%.2f", speed);

    std::cout << "\n▶ 正在播放（画质: " << clarity << " / 速度: x" << speedBuf << "）\n";
    std::cout << "  mpv 快捷键：← → 快退/快进  空格 暂停  q 退出\n\n";

    pid_t pid = fork();
    if (pid == 0) {
        // 子进程：执行 mpv
        execlp(
            "mpv", "mpv",
            url.c_str(),
            "--ytdl=yes",                                   // 启用 yt-dlp 支持
            ("--ytdl-format=" + formatStr).c_str(),        // 画质
            ("--speed=" + std::string(speedBuf)).c_str(),  // 倍速
            "--no-terminal",                                // 不占用终端输出（避免刷屏）
            "--term-osd-bar",                               // 终端里显示进度条
            "--term-playing-msg=正在加载...",
            NULL
        );
        // 如果 mpv 没装，提示用户
        perror("❌ 启动 mpv 失败，请确认已安装 mpv：sudo apt install mpv");
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, nullptr, 0);
        std::cout << "\n✅ 播放结束\n";
    } else {
        perror("fork 失败");
    }
}
