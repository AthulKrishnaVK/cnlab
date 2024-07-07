#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char rcv[100], fileread[1000];
    FILE *fp;

   
    memset(rcv, '\0', sizeof(rcv));
    memset(fileread, '\0', sizeof(fileread));

 
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror("Error while creating socket");
        return -1;
    }
    printf("Socket created successfully\n");

    char address[20] = "127.0.0.1";
    int port;
    printf("Enter the port address: ");
    scanf("%d", &port);


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(address);

    
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Couldn't bind to the port");
        return -1;
    }
    printf("Done with binding\n");


    if (listen(socket_desc, 1) < 0) {
        perror("Error while listening");
        return -1;
    }
    printf("Listening for incoming connections...\n");

  
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, (socklen_t*)&client_size);
    if (client_sock < 0) {
        perror("Can't accept");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    
    if (recv(client_sock, rcv, sizeof(rcv), 0) < 0) {
        perror("Couldn't receive");
        close(client_sock);
        return -1;
    }
    printf("Requested file: %s\n", rcv);

  
    fp = fopen(rcv, "r");
    if (!fp) {
        send(client_sock, "error", strlen("error"), 0);
        close(client_sock);
        close(socket_desc);
        return 0;
    }

    
    while (fgets(fileread, sizeof(fileread), fp)) {
        if (send(client_sock, fileread, strlen(fileread), 0) < 0) {
            perror("Can't send file contents");
            fclose(fp);
            close(client_sock);
            close(socket_desc);
            return -1;
        }
    }
    send(client_sock, "completed", strlen("completed"), 0);

   
    fclose(fp);
    close(client_sock);
    close(socket_desc);

    return 0;
}

