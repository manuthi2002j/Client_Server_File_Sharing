# Server-Client File Download Application

## Instructions

### To Compile The Server Code
```bash
make -f makeserver
```

### To Run Server Code
```bash
./server.out 127.0.0.1
```

### To Compile The Client Code
```bash
make -f makeclient
```

### To Run Client Code
```bash
./client.out 127.0.0.1
```

---

## Important Notes

1. **Terminal Setup**: To run the codes, open two separate terminal windows: one for the server and one for the client. To connect multiple clients, open additional terminal windows for each client.

2. **Server State**: Ensure the server is in a listening state (running) before starting any client connections.

3. **Downloading Files**: To download a file from the server, the user must enter the corresponding file number instead of the file name.

4. **Client Reconnection**: After downloading a file, the client will automatically disconnect. To download another file, the client must reconnect to the server.

---

## Example Workflow

1. **Start the Server**:
    ```bash
    ./server.out 127.0.0.1
    ```

2. **Connect a Client**:
    ```bash
    ./client.out 127.0.0.1
    ```

3. **Download a File**:
    - Enter the file number provided by the server when prompted.

4. **Reconnect to Download Another File**:
    - Restart the client to establish a new connection with the server.
