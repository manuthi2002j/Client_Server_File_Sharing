#include "sock.h" 

void recv_file(int sockfd, char *filename) {
	int bytes_received;  // Holds the number of bytes received from the server.
	int last_printed_progress = 0;  // Tracks last printed progress percentage.
	FILE *fp;  // File pointer to write received data.
	char *direc = "/home/lokuballa/Recv_file";  // Directory to save received files.
	char full_path[BUFFER_SIZE];  // Full path for the file to be saved.
	char buffer[BUFFER_SIZE];  // Buffer to hold received data.
	long file_size;  // Size of the file to be received.
	long total_bytes_received = 0;  // Total bytes received so far.
	float progress = 0;  // Progress percentage of the file download.

	bzero(buffer, BUFFER_SIZE);  
	bzero(full_path, BUFFER_SIZE);  

	// Create the full path for the file to be saved.
	snprintf(full_path, sizeof(full_path), "%s/%s", direc, filename);

	// Open the file in binary write mode.
	fp = fopen(full_path, "wb");
	if (fp == NULL) {
		perror("Failed to open file\n");
		exit(1);
	}

	// Receive file size from the server.
	if (recv(sockfd, &file_size, sizeof(file_size), 0) <= 0) {
		perror("Failed to receive file size");
		exit(1);
	}

	printf("Downloading file of size: %ld bytes\n", file_size);

	// Receive the file in chunks and write to file.
	while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
		fwrite(buffer, sizeof(char), bytes_received, fp);  // Write data to file.
		total_bytes_received += bytes_received;  // Update total bytes received.

		// Calculate and display download progress.
		progress = ((float)total_bytes_received / file_size) * 100;
		if ((int)progress >= last_printed_progress + 10) {
			last_printed_progress = (int)progress;
			printf("Download progress: %d%%\n", last_printed_progress);  // Print progress every 10%.
			fflush(stdout);
		}
		bzero(buffer, BUFFER_SIZE);  // Clear buffer after each chunk.
	}

	if (bytes_received == -1) {
		perror("Error receiving file\n");
		exit(1);
	} else {
		printf("File received successfully\n");
	}

	fclose(fp);  // Close the file after writing is complete.
}

