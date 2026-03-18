# 基础配置（根据你的项目修改）
# 1. 可执行文件名称（自定义）
TARGET := bilibili_downloader
# 2. 源码文件（如果源码分散，可追加，比如 src/*.cpp）
SRCS := main.cpp curl_pool.cpp memory_pool.cpp thread_pool.cpp
# 3. 目标文件（自动推导，无需改）
OBJS := $(SRCS:.cpp=.o)
# 4. 编译器（默认g++，也可换clang++）
CXX := g++
# 5. 编译选项（核心：启用C++11、警告、链接依赖库）
CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -g  # -O2优化速度，-g保留调试信息
# 6. 链接库（curl + 线程 + 内存映射等）
LDFLAGS := -lcurl -lpthread -lrt  # -lrt：部分Linux系统需要（mmap相关）

# 核心目标：编译可执行文件
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 编译单个cpp文件为o文件（自动推导规则）
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译产物（make clean）
clean:
	rm -rf $(TARGET) $(OBJS) *.o *.dSYM  # *.dSYM是macOS调试文件

# 安装可执行文件到系统路径（sudo make install）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# 卸载（sudo make uninstall）
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# 伪目标（避免与同名文件冲突）
.PHONY: all clean install uninstall
