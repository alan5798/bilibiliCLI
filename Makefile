CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LIBS     = -lcurl

TARGET  = bili
SRCS    = bili.cpp search.cpp play.cpp file.cpp help.cpp input.cpp
OBJS    = $(SRCS:.cpp=.o)

.PHONY: all clean install-deps

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

# Ubuntu / Debian 一键安装依赖
install-deps:
	sudo apt update
	sudo apt install -y mpv yt-dlp nlohmann-json3-dev libcurl4-openssl-dev
