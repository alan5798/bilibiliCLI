#include "input.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void getCommand(std::string& cmd, std::string& arg) {
    cmd = "";
    arg = "";

    std::cout << "\nbili> ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        cmd = "q"; // EOF（Ctrl+D）当作退出
        return;
    }

    std::istringstream iss(line);
    iss >> cmd;

    // 参数是命令后面剩余的全部内容（支持多词搜索）
    std::string rest;
    if (std::getline(iss >> std::ws, rest)) {
        arg = rest;
    }

    // 命令转小写
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
}
