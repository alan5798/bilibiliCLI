#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "video.h"
#include "file.h"
void play(std::string url){
    int pipefd[2];
    if(pipe(pipefd)==-1){
        perror("pipe创建失败");
        return;
    }
    int pipesize=128*1024;
    fcntl(pipefd[1],F_SETPIPE_SZ,pipesize);
    int read_fd=pipefd[0];
    int write_fd=pipefd[1];
    //excute yt-dlp
    pid_t pid_yt=fork();
    if(pid_yt==0){//child process
        dup2(write_fd,STDOUT_FILENO);
        close(read_fd);
        close(write_fd);
        execlp(
            "yt-dlp", 
            "yt-dlp",
            "-o",
            "-",
            url.c_str(),
            NULL
        );
        perror("execlp yt-dlp fail\n");
        exit(1);
    }
    pid_t pid_ffplay=fork();
    if(pid_ffplay==0){
        dup2(read_fd,STDIN_FILENO);
        close(read_fd);
        close(write_fd);
        execlp(
            "ffplay",
            "ffplay",
            "-i","-",
            "-autoexit",
            NULL
        );
        perror("execlp ffplay fail\n");
        exit(1);
    }
    close(read_fd);
    close(write_fd);
    waitpid(pid_yt,nullptr,0);
    waitpid(pid_ffplay,nullptr,0);
}