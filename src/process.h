#pragma once //include this file only once

#include <string>

struct Process{
	int pid;
	std::string name;
	char state; // R = running, S = sleeping, Z = zombie
	long vm_rss_kb; // physical memory used in kB
};
