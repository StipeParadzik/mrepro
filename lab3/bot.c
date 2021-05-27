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
#include <errno.h>

#define MAXLEN 256
void print_usage() {

    printf("Enter two arguments server_ip and server_port\n");
    exit(1);
}

int main(int argc, char **argv) {

    int opt;
    char *server_ip, *server_port;
    char *content = "REG\n";

    struct info {
        char IP[INET_ADDRSTRLEN];
        char PORT[22];
    };

    struct MSG {
        char command;
        struct info ipp[20];
    };


    while ((opt = getopt(argc, argv, "")) != -1) { //ovdje citam argumente sto se posalju pri pozivu programa
        switch (opt)

            printf("There are no options avalaible");
        exit(1);
        break;
    }

    if (argc - optind != 2) {

        print_usage();
    }

    struct MSG msg;

    memset(&msg, '\0', sizeof(msg));

    server_ip = argv[optind];
    server_port = argv[optind + 1];
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = 0;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

    struct addrinfo *res = 0;
    getaddrinfo(server_ip, server_port, &hints, &res);


    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    sendto(fd, content, sizeof(content), 0, res->ai_addr, res->ai_addrlen);

    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    char message[1024];
    memset(&message, '\0', sizeof(message));

	strcpy(message, "\n");
	
    while (1) {

        recvfrom(fd, &msg, sizeof(msg), 0, (struct sockaddr *) &src_addr, &src_addr_len);
        printf("dobio sam poruku %s\n",msg.ipp[0].PORT);

        //TCP
        if (msg.command == '1') {

            int sockfd;
            char buffer[1024];
            char hello[6] = "HELLO\n";
            struct sockaddr_in servaddr;

            int n, len;
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("socket creation failed\n");
                exit(7);
            }
            memset(&servaddr, 0, sizeof(servaddr));

            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons((unsigned short) atol(msg.ipp[0].PORT));
            servaddr.sin_addr.s_addr = inet_addr(msg.ipp[0].IP);

            if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
                printf("\nError connect\n");
            }
            memset(buffer, 0, sizeof(buffer));
            write(sockfd, hello, sizeof(hello));
            printf("Message from server : ");
            read(sockfd, buffer, sizeof(buffer));
            strcpy(message, buffer);
            close(sockfd);

        }
            //UDP
        else if (msg.command == '2') {


            char hello[6] = "HELLO\n";


            int sockfd;
            char buffer[1024];
            struct sockaddr_in servaddr;

            int n, len;

            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                printf("socket creation failed");
                exit(5);
            }
			
            memset(&servaddr, 0, sizeof(servaddr));

            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons((unsigned short) atol(msg.ipp[0].PORT));
            servaddr.sin_addr.s_addr = inet_addr(msg.ipp[0].IP);

            sendto(sockfd, (const char *) hello, strlen(hello), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));
            printf("message from server: ");
            n = recvfrom(sockfd, (char *) buffer, 1024, 0, (struct sockaddr *) &servaddr, &len);
            strcpy(message, buffer);
            close(sockfd);


        }

            //Slanje poruka
        else if (msg.command == '3') {
			
			char poruke[10][100];//nije najbolje rijesenje za poruke jer znam da jedna moze biti duzine 1024, a moze ih i biti 512 sa duzinom jedan
			int broj_poruka=0;
			int prvi=0;
			int drugi = 0;
			
			
			for(int j = 0; j < strlen(message);++j){
				if(message[j] == ':' && prvi == 0 && drugi == 0){
					drugi = j;
					memset(poruke[broj_poruka],0, sizeof(poruke[broj_poruka]));
					int nulti = 0;
					for (int z = prvi; z < drugi;++z){
						char c = message[z];
						poruke[broj_poruka][nulti] = c;
						nulti++;
						}
						poruke[broj_poruka][nulti] = '\n';
						nulti++;
						
					broj_poruka++;
					prvi = drugi+1;
					}
					else if (message[j] == ':' || message[j] == '\n'){
						memset(poruke[broj_poruka],0, sizeof(poruke[broj_poruka]));
						drugi = j;
						int nulti =0;
						for (int z = prvi; z < drugi;++z){
							char c= message[z];
						poruke[broj_poruka][nulti] = c;
						nulti++;
						}
						poruke[broj_poruka][nulti] = '\n';
						nulti++;
					
						broj_poruka++;
						prvi = drugi+1;
						
						}
				}
				
				for (int k = 0;k<broj_poruka;++k){
					printf("%s\n",poruke[k]);
					}
				
				if(broj_poruka == 0){
					strcpy(poruke[broj_poruka],message);
					}
			printf("Ovo je broj poruka: %d\n",broj_poruka);
			
            int opt = 1;
            int master_socket, addrlen, new_socket, client_socket[20], max_clients = 0, activity, valread, sd;
            int max_sd;
            master_socket = fd;
            max_sd = master_socket;
            fd_set readfds;
            int postavljeno = 0;
            int nready;
            struct addrinfo *rez[20];
			if (broj_poruka == 0)broj_poruka = 1;
			
            for (int i = 0; i < 20; ++i) {
				//stvaranje socketa za svaku od adresa na koju treba slati podatke i prvo slanje podataka
                if (strcmp(msg.ipp[i].IP, "") == 0)break;
                max_clients++;
                struct addrinfo hints;
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_DGRAM;
                hints.ai_protocol = 0;
                hints.ai_flags = AI_ADDRCONFIG;
                struct addrinfo *res = 0;
                int err;
                char *port = "5678";//ne salje mi na ovaj port nikad osim kada ga ovako upisem, kada citam podatke sa msg[i]port pisu neka slova a ne ovaj port

                err = getaddrinfo(msg.ipp[i].IP, msg.ipp[i].PORT, &hints, &res);
				
                if (err != 0) {
                    puts("getaddrinfo error");
                    exit(10);
                }
                int ff = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                char broadcast = '1';
                if(strcmp(msg.ipp[i].IP, "255.255.255.255") == 0){
                if(setsockopt(ff,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast))<0){
					printf("broadcast\n");// broadcast 
					}}
                client_socket[i] = ff;
                if (client_socket[i] > max_sd)max_sd = client_socket[i];
                if (ff == -1) {
                    puts("socket error");
                    exit(11);
                }
                rez[i] = res;
                
                if (sendto(ff, message, sizeof(message), 0, res->ai_addr, res->ai_addrlen) == -1) {
                    puts("send to error");
                    exit(12);
                }
                
			sleep(1);
			
            }

            struct timeval tv;
            int val;


            FD_ZERO(&readfds);
            volatile int timer = 0;//timer za 100sekundi


            while (1) {

                tv.tv_sec = 1;
                tv.tv_usec = 0;

                do {

                    FD_SET(master_socket, &readfds);

                    for (int i = 0; i < max_clients; ++i) {

                        FD_SET(client_socket[i], &readfds);
                    }


                    val = select(max_sd + 1, &readfds, NULL, NULL, &tv);


                    if (FD_ISSET(master_socket, &readfds)) {


                        bzero(&msg, sizeof(msg));
                        int n = recvfrom(master_socket, &msg, sizeof(msg), 0, (struct sockaddr *) &src_addr,
                                         &src_addr_len);

                        if (msg.command == '0' || msg.command == '4') {
                            exit(0);
                        }


                    }

                    for (int i = 0; i < max_clients; ++i) {
                        if (FD_ISSET(client_socket[i], &readfds)) {
						//ako poruku dobije od onoga kome salje podatke
                            exit(19);

                        }

                    }
                } while (val != 0);
                if (timer > 100)break;//primitivna verzija timera
                timer++;

				for(int j=0;j<broj_poruka;++j){
                for (int i = 0; i < max_clients; ++i) {//slanje poruka
                    if (sendto(client_socket[i], poruke[j], sizeof(poruke[j]), 0, rez[i]->ai_addr, rez[i]->ai_addrlen) ==
                        -1) {
                        puts("send to error");
                        exit(12);
                    }
                } 
                sleep(1);
              }



            }

        } else if (msg.command == '4') {
			continue;

        } else if (msg.command == '0') {
            exit(0);

        } else {
            puts("nepoznata naredba\n");
            continue;
        }

    }

}
