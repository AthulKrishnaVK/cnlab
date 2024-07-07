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

  
    memset(name, '\0', sizeof(name));
    memset(fname, '\0', sizeof(fname));
    memset(rcvg, '\0', sizeof(rcvg));


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


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(address);

 
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect");
        return -1;
    }
    printf("Connected with server successfully\n");

   
    printf("Enter the existing file name: ");
    scanf("%s", name);
    printf("Enter the new file name: ");
    scanf("%s", fname);

    fp = fopen(fname, "w");
    if (!fp) {
        perror("Unable to create file");
        return -1;
    }

  
    if (send(socket_desc, name, strlen(name), 0) < 0) {
        perror("Unable to send message");
        fclose(fp);
        return -1;
    }

    
    while (1) {
        int s = recv(socket_desc, rcvg, sizeof(rcvg) - 1, 0); 
        if (s <= 0) {
            break;  
        }
        
        rcvg[s] = '\0'; 

        if (strcmp(rcvg, "error") == 0) {
            printf("File is not available.\n");
            break;
        }
        if (strcmp(rcvg, "completed") == 0) {
            printf("File is transferred.\n");
            break;
        }
        
        fputs(rcvg, stdout);
        fputs(rcvg, fp); 
    }

    fclose(fp);
    close(socket_desc);

    return 0;
}

