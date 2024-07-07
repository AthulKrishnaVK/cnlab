#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int socket_desc;
    struct sockaddr_in server_addr;
    char name[1000], fname[1000], rcvg[1000];
    FILE *fp;

    // Clean buffers:
    memset(name, '\0', sizeof(name));
    memset(fname, '\0', sizeof(fname));
    memset(rcvg, '\0', sizeof(rcvg));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror("Unable to create socket");
        return -1;
    }
    printf("Socket created successfully\n");

    int port;
    char address[] = "127.0.0.1";
    printf("Enter the port: ");
    scanf("%d", &port);

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(address);

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect");
        return -1;
    }
    printf("Connected with server successfully\n");

    // Get input from the user:
    printf("Enter the existing file name: ");
    scanf("%s", name);
    printf("Enter the new file name: ");
    scanf("%s", fname);

    fp = fopen(fname, "w");
    if (!fp) {
        perror("Unable to create file");
        return -1;
    }

    // Send the file name to server:
    if (send(socket_desc, name, strlen(name), 0) < 0) {
        perror("Unable to send message");
        fclose(fp);
        return -1;
    }

    // Receive the file content from server:
    while (1) {
        int s = recv(socket_desc, rcvg, sizeof(rcvg) - 1, 0); // Receive up to sizeof(rcvg) - 1 bytes
        if (s <= 0) {
            break;  // Handle receive errors or connection termination
        }
        
        rcvg[s] = '\0';  // Null-terminate received data

        if (strcmp(rcvg, "error") == 0) {
            printf("File is not available.\n");
            break;
        }
        if (strcmp(rcvg, "completed") == 0) {
            printf("File is transferred.\n");
            break;
        }
        
        fputs(rcvg, stdout);
        fputs(rcvg, fp);  // Write received data to the file
    }

    fclose(fp);
    close(socket_desc);

    return 0;
}

