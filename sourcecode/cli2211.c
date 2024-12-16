#include "sock.h"  

int main(int argc, char **argv) {

    int clifd, a, selected_file_number; // a: status of connection, selected_file_number: for selecting a file to download.
    char buffer[BUFFER_SIZE]; // Buffer to store data received from the server.
    char send_buffer[BUFFER_SIZE]; // Buffer to store data to send to the server.

    struct sockaddr_in cliaddr; // Struct to store server address details.

    // Create a socket 
    if ((clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }
    
    bzero(&cliaddr, sizeof(cliaddr)); // Zero out the server address struct.

    // Set up the server address (IPv4, port 2211, and IP address from the command line argument).
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(2211); 
    cliaddr.sin_addr.s_addr = inet_addr(argv[1]); // Server IP address provided as a command-line argument.

    // Connect to the server. 
    a = connect(clifd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    if (a == 0) {
        printf("connection successful\n");
    } else {
        perror("connect error");
        exit(1);
    }

    bzero(buffer, BUFFER_SIZE); // Clear the buffer.
    
    // Read the list of available files from the server and display them.
    read(clifd, buffer, BUFFER_SIZE);
    printf("Available files:\n%s\n", buffer);

    printf("Enter the number of the file you want to download: ");
    scanf("%d", &selected_file_number);

    // Send the selected file number to the server.
    snprintf(send_buffer, sizeof(send_buffer), "%d", selected_file_number);
    write(clifd, send_buffer, strlen(send_buffer));

    bzero(buffer, BUFFER_SIZE); 
    
    // Read the server's response.
    read(clifd, buffer, BUFFER_SIZE);

   
    recv_file(clifd, buffer);
}

