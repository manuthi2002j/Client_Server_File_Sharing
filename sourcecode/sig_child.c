#include "sock.h"
#include <sys/wait.h>  // Include for handling child processes.

void sig_child(int signo) {

	pid_t pid;  
	int stat;  // Variable to store child process status.

	printf("SIGCHLD received\n"); 

	// Wait for all child processes that have terminated (non-blocking).
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);  // Print terminated child process ID.

	return;  
}

