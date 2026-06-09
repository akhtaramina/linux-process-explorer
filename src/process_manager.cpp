#include "process.h"

#include <dirent.h>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

bool read_process(int pid, Process& proc);
std::vector<Process> get_all_processes(){
	std::vector<Process> processes;
	DIR* proc_dir = opendir("/proc");
	if(!proc_dir){
		return processes; // return empty if cannot open file
	}
	struct dirent* entry;
	while((entry = readdir(proc_dir)) != nullptr){
		std::string name(entry->d_name);
		bool is_pid = !name.empty() && std::all_of(name.begin(), name.end(), ::isdigit);
		if(is_pid){
			int pid = std::stoi(name);
			Process proc{};
			if(read_process(pid, proc)){
				processes.push_back(proc);
			}
		}
	}
	closedir(proc_dir);
	return processes;
}
