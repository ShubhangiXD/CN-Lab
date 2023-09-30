// Sender
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define TIMEOUT_SEC 2

typedef struct {
    int seq_num;
    char data[100];
} Packet;

int main() {
    int senderSocket;
    struct sockaddr_in receiverAddr;

    // Create UDP socket
    senderSocket = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(12345);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    int base = 0;
    int next_seq_num = 0;
    Packet packets[WINDOW_SIZE];

    while (1) {
        // Send packets within the window
        while (next_seq_num < base + WINDOW_SIZE) {
            Packet packet;
            packet.seq_num = next_seq_num;
            sprintf(packet.data, "Packet %d", next_seq_num);

            packets[next_seq_num % WINDOW_SIZE] = packet;
            sendto(senderSocket, &packet, sizeof(packet), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));

            printf("Sent: %s\n", packet.data);

            next_seq_num++;
        }

        // Receive ACKs
        fd_set read_fds;
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(senderSocket, &read_fds);

        if (select(senderSocket + 1, &read_fds, NULL, NULL, &timeout) > 0) {
            int ack_num;
            recvfrom(senderSocket, &ack_num, sizeof(ack_num), 0, NULL, NULL);

            if (ack_num >= base) {
                printf("Received ACK: %d\n", ack_num);
                base = ack_num + 1;
            }
        } else {
            // Timeout, retransmit
            printf("Timeout, retransmitting...\n");
            next_seq_num = base;
        }
    }

    close(senderSocket);
    return 0;
}