#include "server.h"


void Server::launchSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(1337);

    if (bind(sockfd, (const struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void Server::RunReviewer() {
    int res = recv(sockfd, &data, sizeof(int), MSG_WAITALL);
    if (res == -1) {
        perror("recv failed with error\n");
        exit(EXIT_FAILURE);
    }
    if ((data > 99) && (data % 32 == 0)) {
        std::cout << "Data received" << std::endl;
    } else {
        std::cout << "Error with data receiving" << std::endl;
    }
}