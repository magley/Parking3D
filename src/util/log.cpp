#include "log.h"
#include <stdio.h>
#include <sstream>
#include <chrono>
#include <ctime> 

void log(const std::string& msg) {
	auto now = std::chrono::system_clock::now();
	auto now_t = std::chrono::system_clock::to_time_t(now);
	char buffer[50];
	ctime_s(buffer, sizeof(buffer), &now_t);
	std::string now_str = std::string(buffer);
	now_str.erase(std::remove(now_str.begin(), now_str.end(), '\n'), now_str.cend());

	std::stringstream ss;
	ss << "[" << now_str << "]   " << msg << "\n";
	printf("%s", ss.str().c_str());
}
