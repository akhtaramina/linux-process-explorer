#include "process.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

std::vector<Process> get_all_processes();
//signal handling
volatile sig_atomic_t running = 1;

void handle_sigint(int){
	running = 0;
}

void print_processes(const std::vector<Process>& processes) {
	std::cout << std::left
	      << std::setw(8)  << "PID"
	      << std::setw(20) << "NAME"
	      << std::setw(10) << "STATE"
	      << std::setw(15) << "MEMORY(KB)"
	      << "\n";

	std::cout << std::string(53, '-') << "\n";

	for (const Process& p : processes) {
		std::string state_label;
		switch (p.state) {
		    case 'R': state_label = "Running";  break;
	    	case 'S': state_label = "Sleeping"; break;
	    	case 'Z': state_label = "Zombie";   break;
	    	case 'D': state_label = "Waiting";  break;
	    	default:  state_label = "Other";    break;
	}

	std::string display_name = p.name;
	if (display_name.length() > 18) {
	    	display_name = display_name.substr(0, 16) + "..";
	}

	std::cout << std::left
	          << std::setw(8)  << p.pid
	          << std::setw(20) << display_name
	          << std::setw(10) << state_label
	          << std::setw(15) << p.vm_rss_kb
	          << "\n";
	}
}

void run_ps_via_pipe(){
	int pipefd[2];

	if (pipe(pipefd) == -1){
		std::cerr << "pipe() failed\n" ;
		return;
	}
	pid_t pid = fork();
	if (pid < 0){
		std::cerr << "fork() failed\n";
		return;
	}
	if (pid == 0){
		// redirect stdout to the write end of the pipe
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		// replace this process with "ps aux"
		execlp("ps", "ps", "aux", nullptr);

		std::cerr << "exex failed\n";
		_exit(1);
	}
	else{
		close(pipefd[1]); // parent doesnt need write end

		// read from pipe line by line
		FILE* stream = fdopen(pipefd[0], "r");
		char buffer[256];
		int lines = 0;

		std::cout << "\n ---- ps aux output (first 5 lines via pipe)----\n";
		while (fgets(buffer, sizeof(buffer), stream) && lines < 5){
			std::cout << buffer;
			lines++;
		}

		fclose(stream);
		waitpid(pid, nullptr, 0);
	}
}

int main(){
	// register signal handler for Ctrl+C
	std::signal(SIGINT, handle_sigint);

	std::cout << "Linux Process Explorer\n";
	std::cout << "Press Ctrl+C to exit\n\n";

	run_ps_via_pipe();

	std::cout << "\n---- Processes from /proc ------\n\n";

	std::vector<Process> processes = get_all_processes();
	print_processes(processes);

	std::cout << "\nDone. Total processes: " << processes.size() << "\n";

	return 0;

}
