#pragma once
#include <string>

// clarity: "360", "480", "720", "1080", "best"
// speed:   0.5 / 1.0 / 1.5 / 2.0
void play(const std::string& url, const std::string& clarity = "best", double speed = 1.0);
