#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <err.h>

#define STDIN 0

#define MAXLEN 256

void print_usage() {

    printf("Enter two arguments server_ip and server_port\n");
    exit(1);
}


int main(int argc, char **argv) {


    int opt;
    char *tport = "1234", *uport = "1234", payload[1024] = "";


    while ((opt = getopt(argc, argv, "p:t:l:")) != -1) {


        switch (opt) {


            case 'p':
                strcpy(payload, optarg);
                strncat(payload, "\n", 1);
                break;


            case 't':
                tport = optarg;
                break;
            case 'u':
                uport = optarg;
                break;

            default:
                printf("server [-t tcp_port] [-u udp_port] [-p popis]\n");
                return 1;


        }


    }


//--------------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------------------------
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char buffer[1024];
    char message[1024] = "";
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    void sig_chld(int);


    //TCP

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short) atoi(tport));

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, 10);

    servaddr.sin_port = htons((unsigned short) atoi(uport));
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(udpfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    FD_ZERO(&rset);

    if (listenfd > udpfd) {
        maxfdp1 = listenfd + 1;
    } else maxfdp1 = udpfd + 1;

    if (maxfdp1 - 1 < STDIN)maxfdp1 = STDIN + 1;

    char naredba[256];

    while (1) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        FD_SET(STDIN, &rset);

        nready = select(maxfdp1, &rset, NULL, NULL, NULL);


        if (FD_ISSET(STDIN, &rset)) {
            fgets(naredba, 256, stdin);
            if (strcmp(naredba, "PRINT\n") == 0) {
                puts(payload);

            } else if (strstr(naredba, "SET ") != NULL) {

                for (int i = 0; i < strlen(naredba) - 4; ++i) {
                    payload[i] = naredba[i + 4];

                    if (naredba[i + 4] == '\n' || naredba[i + 4] == '\0')
                        break;
                }

            } else if (strcmp(naredba, "QUIT\n") == 0) {
                exit(0);
            }

        }

        if (FD_ISSET(listenfd, &rset)) {
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
            if ((childpid = fork()) == 0) {
                close(listenfd);
                bzero(buffer, sizeof(buffer));
                read(connfd, buffer, sizeof(buffer));
                if (strcmp(buffer, "HELLO\n") == 0) {
                    strcpy(message, payload);
                    strcat(message, "\n");
                    write(connfd, (const char *) message, sizeof(buffer));
                }
                close(connfd);
                exit(0);
            }
            close(connfd);
        }


        if (FD_ISSET(udpfd, &rset)) {

            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            n = recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cliaddr, &len);

            if (strcmp(buffer, "HELLO\n") == 0) {
                puts(buffer);
                strcpy(message, payload);
                sendto(udpfd, (const char *) message, sizeof(message), 0, (struct sockaddr *) &cliaddr,
                       sizeof(cliaddr));
            }

        }

    }

}
