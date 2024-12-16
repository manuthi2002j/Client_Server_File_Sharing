#include "sock.h"  

void log_info(FILE *log_file, struct sockaddr_in *clisock, const char *file_name, int trans_status) {

	time_t now;  // Holds the current time.
	struct tm *local_time;  // Structure to hold broken-down time.
	char time_str[100];  // Buffer to hold formatted time string.

	// Get the current time.
	time(&now);

	// Convert time to a readable format.
	local_time = localtime(&now);

	// Format the time as a string (YYYY-MM-DD HH:MM:SS).
	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);

	// Log client IP address, port, connection time, requested file, and transfer status.
	fprintf(log_file, "Client IP: %s\n", inet_ntoa(clisock->sin_addr));
	fprintf(log_file, "Client Port: %d\n", ntohs(clisock->sin_port));
	fprintf(log_file, "Connected Time: %s\n", time_str);
	fprintf(log_file, "Requested File: %s\n", file_name);
	fprintf(log_file, "Transfer Status: %s\n", trans_status ? "Failed" : "Successful");
	fprintf(log_file, "----------------------------------------------------------------------------\n");

	// Flush the log file to ensure data is written immediately.
	fflush(log_file);
}

