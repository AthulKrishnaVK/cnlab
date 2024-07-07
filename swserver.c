#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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
    struct sockaddr_in servaddr, cli;
    socklen_t addr_size;
    Frame frame_recv, frame_send;
    int frame_id = 0;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cli, 0, sizeof(cli));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Bind successful\n");

    addr_size = sizeof(cli);

    while (1) {
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&cli, &addr_size);
        if (f_recv_size > 0 && frame_recv.frame_kind == 1 && frame_recv.sq_no == frame_id) {
            printf("[+]Frame Received: %s\n", frame_recv.packet.data);

            frame_send.sq_no = 0;
            frame_send.frame_kind = 0;
            frame_send.ack = frame_recv.sq_no + 1;

            sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&cli, addr_size);
            printf("[+]Ack Sent\n");
        } else {
            printf("[-]Frame Not Received\n");
        }
        frame_id++;
    }

    close(sockfd);
    return 0;
}

