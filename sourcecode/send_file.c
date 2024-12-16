#include "sock.h" 

int send_file(FILE *fp, int sockfd) {

	char data[BUFFER_SIZE];  // Buffer to hold file data.
	long file_size;  // Variable to hold the size of the file.

	bzero(data, BUFFER_SIZE);  

	// Get the file size by moving the file pointer to the end.
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);  // Get the file size.
	rewind(fp);  // Reset the file pointer to the beginning.

	// Send the file size to the client.
	if (send(sockfd, &file_size, sizeof(file_size), 0) == -1) {
		perror("Failed to send file size");
		exit(1);
	}

	// Read the file in chunks and send each chunk to the client.
	while (fread(data, sizeof(char), BUFFER_SIZE, fp) > 0) {

		if (send(sockfd, data, BUFFER_SIZE, 0) == -1) {
			perror("Failed to send file");
			exit(1);
		}

		bzero(data, BUFFER_SIZE);  // Clear the buffer after each send.
	}

	printf("File sent successfully\n");

	// Shutdown the socket's write function.
	shutdown(sockfd, SHUT_WR);

	return 0;
}

