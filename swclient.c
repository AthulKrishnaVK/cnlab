#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

typedef struct packet {
    char data[1024];
} Packet;

typedef struct frame {
    int frame_kind;  // 0: Ack, 1: Data
    int sq_no;
    int ack;
    Packet packet;
} Frame;

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t addr_size;
    Frame frame_recv, frame_send;
    int frame_id = 0;
    int ack_recv = 1;
    char buffer[1024];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    addr_size = sizeof(servaddr);

    while (1) {
        if (ack_recv == 1) {
            frame_send.sq_no = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;

            printf("Enter Data: ");
            scanf("%s", buffer);
            strcpy(frame_send.packet.data, buffer);

            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&servaddr, addr_size);
            printf("[+]Frame Sent\n");
        }

        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr*)&servaddr, &addr_size);
        if (f_recv_size > 0 && frame_recv.frame_kind == 0 && frame_recv.ack == frame_id + 1) {
            printf("[+]Ack Received\n");
            ack_recv = 1;
        } else {
            printf("[-]Ack Not Received\n");
            ack_recv = 0;
        }
        frame_id++;
    }

    close(sockfd);
    return 0;
}

