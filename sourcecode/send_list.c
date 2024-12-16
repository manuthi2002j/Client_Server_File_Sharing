#include "sock.h"  
void send_list(int sockfd) {

	struct dirent *de;  // Directory entry structure.
	DIR *dr = opendir("/home/lokuballa/File");  // Open directory for reading.
	char file_list[BUFFER_SIZE];  // Buffer to store the list of files.
	int file_number = 1;  // Counter for file numbering.
	char file_entry[256];  // Buffer for each file entry.

	bzero(file_list, BUFFER_SIZE); 

	if (dr == NULL) {
		printf("Could not open directory");
		exit(1);
	}

	// Read each file in the directory.
	while ((de = readdir(dr)) != NULL) {

		// Ignore '.' and '..' entries.
		if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {

			bzero(file_entry, 256);  // Clear the file entry buffer.
			
			// Format file entry as "1. filename".
			snprintf(file_entry, sizeof(file_entry), "%d. %s\n", file_number, de->d_name);
			strcat(file_list, file_entry);  // Append to the file list.
			file_number++;  
		}
	}

	closedir(dr);  // Close the directory after reading.

	// Send the list of files to the client.
	if (send(sockfd, file_list, strlen(file_list), 0) == -1) {
		printf("Failed to send file list");
		exit(1);
	}
}

