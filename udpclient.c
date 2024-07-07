#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2000
#define MAX_MESSAGE_SIZE 2000

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[MAX_MESSAGE_SIZE], client_message[MAX_MESSAGE_SIZE];
    int recv_size, server_addr_len;


    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

  
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
   printf("Enter message: ");
    fgets(client_message, sizeof(client_message), stdin);
    

    size_t len = strlen(client_message);
    if (len > 0 && client_message[len - 1] == '\n') {
        client_message[len - 1] = '\0';
    }
 
    printf("Enter message: ");
    fgets(client_message, MAX_MESSAGE_SIZE, stdin);
    client_message[strcspn(client_message, "\n")] = '\0'; // Remove newline character if present

    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
              (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    printf("Message sent to server\n");

   
    server_addr_len = sizeof(server_addr);
    recv_size = recvfrom(socket_desc, server_message, MAX_MESSAGE_SIZE, 0,
                         (struct sockaddr*)&server_addr, &server_addr_len);
    
    if (recv_size < 0){
        printf("Error while receiving server's message\n");
        return -1;
    }
    server_message[recv_size] = '\0'; // Null-terminate the received message
    printf("Server's response: %s\n", server_message);

    // Close the socket:
    close(socket_desc);

    return 0;
}

