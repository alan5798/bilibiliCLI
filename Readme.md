## 命令行bilibili
### 安装方法
* 安装yt-dlp和ffmepg
```
>sudo apt update
>sudo apt install yt-dlp && sudo apt intall ffmepg
```
* 安装Json解析依赖
```
>sudo apt install nlohmann-json3-dev
```
* 安装curl
```
sudo apt install libcurl4-openssl-dev
```
* 编译
```
 g++ -std=c++11 bili.cpp -o bili -lcurl
```
* 运行
```
./bili
```
