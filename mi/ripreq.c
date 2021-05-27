#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <err.h>

#define PORT "520"


int main(int argc, char **argv) {


    if (argc != 2) {
        printf("Usage: ripreq IP_address\n");
        exit(3);
    }

    char *IP_adresa_usmjeritelja = argv[1]; // ip adresa

//definiranje struktura imam 2 puta RIPP jer kada prvi put šaljem ovo strukturu potreba mi je samo jedan RIPentry
    struct RIPentry {
        unsigned char AFI[2];
        unsigned char RT[2];
        unsigned char IPA[4];
        unsigned char SM[4];
        unsigned char NH[4];
        unsigned char M[4];
    };

    struct RIPP {
        unsigned char command[1];
        unsigned char Version[1];
        unsigned char MBZ[2];
        struct RIPentry entry[25];
    };


    struct RIPPs {
        unsigned char command[1];
        unsigned char Version[1];
        unsigned char MBZ[2];
        struct RIPentry entry[1];
    };



//----


    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_ADDRCONFIG;

    struct addrinfo *res = 0;

    int erro = getaddrinfo(IP_adresa_usmjeritelja, PORT, &hints, &res);
    if (erro != 0) {
        err(erro, "getaddrinfo\n");
    }


//---

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1) {
        err(fd, "socket error\n");
    }

//---
    struct RIPPs toSend;
    memset(&toSend, '\0', sizeof(toSend));
    toSend.command[0] = 1;
    toSend.Version[0] = 2;
    toSend.entry[0].M[0] = 0;
    toSend.entry[0].M[1] = 0;
    toSend.entry[0].M[2] = 0;
    toSend.entry[0].M[3] = 0x10;
    memset(toSend.MBZ, '0', sizeof(toSend.MBZ));

//gore postavljam vrijednosti koje trebaju biti poslane


//---

    if (sendto(fd, (const void *) &toSend, sizeof(toSend), 0, res->ai_addr, res->ai_addrlen) == -1) { //sljanje zahtijeva
        err(4, "send error\n");
    }

    struct RIPP reciveM;
    memset(&reciveM, '\0',sizeof(reciveM));
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    if (recvfrom(fd, &reciveM, sizeof(reciveM), 0, (struct sockaddr *) &src_addr, &src_addr_len) == -1) { //primanje odgovora
        err(7, "recive error\n");
    }


    for (int i = 0; i < 25; ++i) {
//ispis
		if(reciveM.entry[i].M[3]=='\0')break;
        printf("%d.%d.%d.%d/%d.%d.%d.%d metrika: %u\n", reciveM.entry[i].IPA[0], reciveM.entry[i].IPA[1],
               reciveM.entry[i].IPA[2], reciveM.entry[i].IPA[3], reciveM.entry[i].SM[0], reciveM.entry[i].SM[1],
               reciveM.entry[i].SM[2], reciveM.entry[i].SM[3], 
               //reciveM.entry[i].M[0],reciveM.entry[i].M[1],
              // reciveM.entry[i].M[2],
               reciveM.entry[i].M[3]);


    }


}
