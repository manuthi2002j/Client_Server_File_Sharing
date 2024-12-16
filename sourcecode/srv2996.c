#include "sock.h"  

int main (int argc, char **argv)
{
    int srvfd;  
    int clifd[MAX_CLIENTS];  
    char buffer[BUFFER_SIZE];  // Buffer to hold data.
    socklen_t clilen;  // Client address length.
    pid_t childpid;  // Process ID for forked child process.
    const char *ptr;  // Pointer for client IP address.
    int i = 0, trans_status;  // Loop counter and transmission status.
    void sig_child(int);  // Function to handle child process signal.
    char file_name[BUFFER_SIZE];  // Buffer to hold the file name.
    FILE *fp;  // File pointer for the file to be sent.
    FILE *log_file;  // File pointer for the log file.
    char *direc = "/home/lokuballa/File";  // Directory to look for files.

    struct sockaddr_in servaddr, cliaddr;  

    // Open log file for appending log data.
    log_file = fopen("server_log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        exit(1);
    }

    // Create server socket.
    if ((srvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));  // Zero out the server address structure.

    // Set server address details.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  
    servaddr.sin_port = htons(2211);  

    // Bind the server socket to the specified IP and port.
    if ((bind(srvfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0) {
        perror("Bind failed\n");
        exit(1);
    }

    // Listen for incoming connections.
    if (listen(srvfd, 5) < 0) {
        perror("Listen failed\n");
        exit(1);
    }
    
    // Handle zombie child processes (terminated processes that haven't been waited for).
    signal(SIGCHLD, sig_child);

    // Loop to accept and handle client connections.
    for (i = 0; i < MAX_CLIENTS; i++) {
        bzero(&cliaddr, sizeof(cliaddr));  // Clear client address structure.
        clilen = sizeof(cliaddr);  

        // Accept incoming client connection.
        if ((clifd[i] = accept(srvfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
            perror("Accept error\n");
            exit(1);
        }

        // Get client's IP address and port number.
        if ((ptr = inet_ntop(AF_INET, &cliaddr.sin_addr, buffer, sizeof(buffer))) == 0) {
            perror("inet_ntop error\n");
            exit(1);
        }
        printf("connection from %s, port %d\n", ptr, ntohs(cliaddr.sin_port));
        ptr = NULL;

        // Fork a child process to handle the client.
        if ((childpid = fork()) == 0) {
            close(srvfd);  
            send_list(clifd[i]);  // Send list of files to the client.

            int file_number, file_count = 1;  // Variables for file selection.
            struct dirent *de;  // Struct to hold directory entry.
            DIR *dr = opendir(direc);  // Open directory for file access.

            if (dr == NULL) {
                perror("Could not open directory");
                exit(1);
            }

            bzero(buffer, BUFFER_SIZE);  

            // Read the selected file number from the client.
            read(clifd[i], buffer, sizeof(buffer));
            file_number = atoi(buffer); 

            // Loop through directory to find the selected file.
            while ((de = readdir(dr)) != NULL) {
                // Skip "." and ".." directories.
                if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                    if (file_count == file_number) {
                        // Construct full file path.
                        char full_path[BUFFER_SIZE];
                        snprintf(full_path, sizeof(full_path), "%s/%s", direc, de->d_name);

                        // Open the selected file.
                        fp = fopen(full_path, "rb");
                        if (fp == NULL) {
                            perror("Error opening file");
                            exit(1);
                        }
                        printf("File opened successfully.\n");

                        // Send the file name to the client.
                        strcpy(file_name, de->d_name);
                        send(clifd[i], file_name, sizeof(file_name), 0);
                    }
                    file_count++;  
                }
            }
            
            // Send the selected file to the client.
            trans_status = send_file(fp, clifd[i]);

            // Log the file transfer information.
            log_info(log_file, &cliaddr, file_name, trans_status);

            // Close the file and client connection.
            fclose(fp);
            close(clifd[i]);
            exit(0);  // Terminate the child process.
        }

        
        close(clifd[i]);
    }

    return 0;
}

