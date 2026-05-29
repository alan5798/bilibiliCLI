# bilibiliCLI

在终端里搜索和播放 B 站视频的命令行工具。

## 依赖

```bash
make install-deps
# 等价于：
sudo apt install -y mpv yt-dlp nlohmann-json3-dev libcurl4-openssl-dev
```

> **注意**：播放器从 `ffplay` 改为 `mpv`，支持进度条、倍速、画质选择，并且内置调用 yt-dlp。

## 编译

```bash
make
```

## 运行

```bash
./bili
```

## 命令

| 命令 | 说明 |
|------|------|
| `s <关键词>` | 搜索视频（支持多词，如 `s 原神 钢琴`） |
| `p <编号>` | 播放搜索结果中的视频，并自动记录到历史 |
| `h` | 显示帮助 |
| `q` | 退出 |
| `speed <倍速>` | 设置播放速度，如 `speed 1.5` |
| `res <分辨率>` | 设置画质，如 `res 720`（360/480/720/1080/best） |
| `history` | 查看历史记录（带编号） |
| `hp <编号>` | 播放历史中的某条视频 |
| `hd <编号>` | 删除历史中的某条记录 |
| `star` | 查看收藏夹（带编号） |
| `ss <编号>` | 收藏搜索结果中的视频 |
| `sp <编号>` | 播放收藏中的某条视频 |
| `sd <编号>` | 删除收藏中的某条记录 |

## 播放快捷键（mpv）

| 按键 | 功能 |
|------|------|
| `←` / `→` | 快退 / 快进 9 秒 |
| `空格` | 暂停 / 继续 |
| `q` | 退出播放 |
| `9` / `0` | 减小 / 增大音量 |
| `f` | 全屏切换 |

## 文件结构

```
bili.cpp      主程序
search.cpp/h  B站搜索 API
play.cpp/h    mpv 播放
file.cpp/h    历史 & 收藏读写
input.cpp/h   命令行解析
help.cpp/h    帮助文本
video.h       VideoInfo 结构体
Makefile
history.txt   自动生成，历史记录
star.txt      自动生成，收藏夹
```
