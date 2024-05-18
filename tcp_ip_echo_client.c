
#include "practice/us_xfr.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 100
#define PORT 12345

void *receive_messages(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    char username[BUF_SIZE]; // Variable to store the username

    printf("Enter your username: ");
    fgets(username, BUF_SIZE, stdin);
    
     // Remove newline character at the end of the username
    username[strcspn(username, "\n")] = 0;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        fprintf(stderr, "socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s on port %d\n",argv[1], PORT);


    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(sfd, buf, numRead) != numRead) {
            fprintf(stderr, "partial/failed write");
            exit(EXIT_FAILURE);
        }

        numRead = read(sfd, buf, BUF_SIZE);
        if (numRead > 0) {
            if (write(STDIN_FILENO, buf, numRead) != numRead) {
                fprintf(stderr, "partial/failed write");
                exit(EXIT_FAILURE);
            }
        } else if (numRead == -1) {
            fprintf(stderr, "read");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "i am closing");
    // Close our socket; server sees EOF
    close(sfd);
    return 0;
}


