#include "process.h"
#include <fstream>
#include <sstream>
#include <string>

// given PID, reads/proc/PID/status and fills a Process struct
// returns true if successful, false if the process no longer exists

bool read_process(int pid, Process& proc){
	// Build the path: /proc/1234/status
	std::string path = "/proc/" + std::to_string(pid) + "/status";
	std::ifstream file(path);
	if(!file.is_open()){
		return false;
	}
	proc.pid = pid;
	std::string line;
	while(std::getline(file, line)){
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "Name:"){
			iss >> proc.name;
		}
		else if (key == "State:"){
			std::string state_str;
			iss >> state_str;
			proc.state = state_str[0];
		}
		else if (key == "VmRSS:"){
			iss >> proc.vm_rss_kb;
		}
	}
	return true;
}
