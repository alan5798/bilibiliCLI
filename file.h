#ifndef HISTORY_H
#define HISTOTY_H
#include <fstream>
#include <iostream>
#include "video.h"
void write(struct VideoInfo v,std::string filename);
void read(std::string filename);
#endif 