// Receiver
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int seq_num;
    char data[100];
} Packet;

int main() {
    int receiverSocket;
    struct sockaddr_in receiverAddr, senderAddr;

    // Create UDP socket
    receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(12345);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(receiverSocket, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));

    int expected_seq_num = 0;

    while (1) {
        Packet packet;
        recvfrom(receiverSocket, &packet, sizeof(packet), 0, (struct sockaddr *)&senderAddr, NULL);

        if (packet.seq_num == expected_seq_num) {
            printf("Received: %s\n", packet.data);
            expected_seq_num++;

            // Send ACK
            sendto(receiverSocket, &packet.seq_num, sizeof(packet.seq_num), 0, (struct sockaddr *)&senderAddr, sizeof(senderAddr));
        }
    }

    close(receiverSocket);
    return 0;
}