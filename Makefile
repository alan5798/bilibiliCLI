# 编译配置
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
# 链接curl库
LIBS = -lcurl

# 你的所有源文件（当前项目实际存在的文件）
SRC = bili.cpp play.cpp search.cpp input.cpp help.cpp file.cpp
# 最终可执行文件名称
TARGET = bili

# 编译命令：直接 make 即可
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# 清理编译产物：make clean
clean:
	rm -f $(TARGET)