#pragma once
#include <string>

// 解析一行用户输入，返回命令字和可选参数
// cmd：命令名（小写），arg：命令后的参数（可能为空）
void getCommand(std::string& cmd, std::string& arg);
