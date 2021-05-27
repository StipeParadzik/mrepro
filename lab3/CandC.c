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
#include <fcntl.h>
#define STDIN 0
#define MAXLEN 256
#define VERSION 23
#define BUFSIZE 8096
#define ERROR 42
#define LOG 44
#define FORBIDDEN 403
#define NOTFOUND 404
void print_usage() {

    printf("CandC [tcp port]\n");
    exit(1);
}

struct {
	char *ext;
	char *filetype;
	} extensions [] = {
		{"gif", "image/gif"},
		{"jpg", "image/jpg"},
		{"html", "text/html"},
		{"pdf", "application/pdf"},
		{"txt", "text/plain"},
		{0,0}
		};
void logger(int type, char *s1, char *s2, int socket_fd)
{
	int fd ;
	char logbuffer[BUFSIZE*2];

	switch (type) {
	case ERROR: (void)sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",s1, s2, errno,getpid()); 
		break;
	case FORBIDDEN: 
		(void)write(socket_fd, "HTTP/1.1 403 Forbidden\nContent-Length: 185\nConnection: close\nContent-Type: text/html\n\n<html><head>\n<title>403 Forbidden</title>\n</head><body>\n<h1>Forbidden</h1>\nThe requested URL, file type or operation is not allowed on this simple static file webserver.\n</body></html>\n",271);
		(void)sprintf(logbuffer,"FORBIDDEN: %s:%s",s1, s2); 
		break;
	case NOTFOUND: 
		(void)write(socket_fd, "HTTP/1.1 404 Not Found\nContent-Length: 136\nConnection: close\nContent-Type: text/html\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n",224);
		(void)sprintf(logbuffer,"NOT FOUND: %s:%s",s1, s2); 
		break;
	case LOG: (void)sprintf(logbuffer," INFO: %s:%s:%d",s1, s2,socket_fd); break;
	}	
	/* No checks here, nothing can be done with a failure anyway */
	if((fd = open("nweb.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		(void)write(fd,logbuffer,strlen(logbuffer)); 
		(void)write(fd,"\n",1);      
		(void)close(fd);
	}
	if(type == ERROR || type == NOTFOUND || type == FORBIDDEN) exit(3);
}

int main(int argc, char **argv) {

    int opt;
    char *server_ip, *tcp_port;
    char *content = "REG\n";

    struct info {
        char IP[INET_ADDRSTRLEN];
        char PORT[22];
    };

    struct MSG {
        char command;
        struct info ipp[20];
    };


    while ((opt = getopt(argc, argv, "")) != -1) {
        switch (opt)

            printf("There are no options avalaible");
        exit(1);
        break;
    }

    if (argc - optind > 1) {

        print_usage();
    } else if (argc - optind == 1) {
        tcp_port = argv[1];

    } else {
        tcp_port = "80";
    }

    struct MSG msg;
    memset(&msg, 0, sizeof(msg));
    char buffer[1024];
    int udp_socket, nready, tcp_socket;
    struct sockaddr_in udp_addr, cliaddr, servaddr;
    bzero(&udp_addr, sizeof(udp_addr));
    char *udp_port = "5555";
    fd_set rset;
    pid_t childpid;
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short) atoi(tcp_port));

    bind(tcp_socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(tcp_socket, 10);
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_addr.sin_port = htons((unsigned short) atoi(udp_port));
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    bind(udp_socket, (struct sockaddr *) &udp_addr, sizeof(udp_addr));
    struct sockaddr_in *clients;
    char naredba[256];
    const int client_number = 50;
    int spaces_left = 0;
    clients = malloc(client_number * sizeof(cliaddr));
    if (!clients) {
        perror("Error allocating memory");
        exit(3);
    }

    memset(clients, 0, sizeof(cliaddr) * client_number);
    int maxfdp = STDIN + 1;
    if (udp_socket + 1 > maxfdp)maxfdp = udp_socket + 1;
    if (tcp_socket + 1 > maxfdp)maxfdp = tcp_socket + 1;
    FD_ZERO(&rset);
    while (1) {

        FD_SET(udp_socket, &rset);
        FD_SET(tcp_socket, &rset);
        FD_SET(STDIN, &rset);
        nready = select(maxfdp, &rset, NULL, NULL, NULL);


        if (FD_ISSET(tcp_socket, &rset)) {

            int len = sizeof(cliaddr);
            int connfd = accept(tcp_socket, (struct sockaddr *) &cliaddr, &len);
            if ((childpid = fork()) == 0) {
                close(tcp_socket);
                bzero(buffer, sizeof(buffer));
                int ret = read(connfd, buffer, sizeof(buffer));




                //primanje naredbe 
                if (strstr(buffer, "GET") != NULL) {
                    int skip = 0;
                    int send = 1;
                    if (strstr(buffer, "/bot/prog_tcp_localhost") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '1';
                        strcpy(msg.ipp[0].IP, "127.0.0.1");
                        strcpy(msg.ipp[0].PORT, "1234");
                    } else if (strstr(buffer, "/bot/prog_tcp") != NULL) {
                        msg.command = '1';
                        strcpy(msg.ipp[0].IP, "10.0.0.20");
                        strcpy(msg.ipp[0].PORT, "1234");
                    } else if (strstr(buffer, "/bot/prog_udp_localhost") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '2';
                        strcpy(msg.ipp[0].IP, "127.0.0.1");
                        strcpy(msg.ipp[0].PORT, "1234");
                    } else if (strstr(buffer, "/bot/prog_udp") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '2';
                        strcpy(msg.ipp[0].IP, "10.0.0.20");
                        strcpy(msg.ipp[0].PORT, "1234");
                    } else if (strstr(buffer, "/bot/run2") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '3';
                        strcpy(msg.ipp[0].IP, "20.0.0.11");
                        strcpy(msg.ipp[0].PORT, "1111");
                        strcpy(msg.ipp[1].IP, "20.0.0.12");
                        strcpy(msg.ipp[1].PORT, "2222");
                        strcpy(msg.ipp[2].IP, "20.0.0.13");
                        strcpy(msg.ipp[2].PORT, "dec-notes");
                    } else if (strstr(buffer, "/bot/run") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '3';
                        strcpy(msg.ipp[0].IP, "127.0.0.1");
                        strcpy(msg.ipp[0].PORT, "vat");
                        strcpy(msg.ipp[1].IP, "localhost");
                        strcpy(msg.ipp[1].PORT, "6789");
                    } else if (strstr(buffer, "/bot/stop") != NULL) {
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '4';
                    } else if (strstr(buffer, "/bot/list") != NULL) {
                        send = 0;
                        skip = 1;
                        int j, file_fd, buflen;
                        long i, len;
                        char *fstr;
                        static char buf[8096 + 1];


                        buflen = strlen(buffer);
                        fstr = (char *) 0;

                        (void) sprintf(buffer,
                                       "HTTP/1.1 200 OK\r\n");

                        strcat(buffer, "Content-Type: text/html\r\n");

                        strcat(buffer, "Content-Length: ");
                        sprintf(buf, "<!DOCTYPE html>\n<html>\n<body>\n<ul>\n");
                        for (i = 0; i < spaces_left; ++i) {

                            char *ip = inet_ntoa(clients[i].sin_addr);
                            strcat(buf, "<li>");
                            strcat(buf, ip);
                            strcat(buf, "</li>\n");

                        }
                        strcat(buf, "</ul>\n</body>\n</html>\r\n\r\n");
                        puts(buffer);
                        puts(buf);
                        char hold[5];
                        sprintf(hold, "%d", strlen(buf));
                        int toSendSize = strlen(buffer);
                        int sent = 0;
                        strcat(buffer, hold);
                        strcat(buffer, "\r\n");
                        strcat(buffer, "Connection: close\r\n\r\n");
                        write(connfd, buffer, strlen(buffer));
                        write(connfd, buf, strlen(buf));


                    } else if (strstr(buffer, "/bot/quit") != NULL) {
                        send = 0;
                        memset(&msg, 0, sizeof(msg));
                        msg.command = '0';
                        puts("Kraj programa");
                        exit(0);
                    } else {
                        
                        puts(buffer);
                        //slanje ostalih filova
                        skip = 1;
                        
                        
                        
                        send = 0;
						int prvi =0 ;
						int drugi = 0;
                    
                    for (int i = 4; i < strlen(buffer);++i){
						
						if(buffer[i] == '/' && prvi ==0 && drugi == 0){prvi = i;
							++i;}
						else if(buffer[i] == ' ' && prvi != 0){drugi = i;
							break;}
						
						
						}
						char lokacija[80];
						memset(lokacija,'\0',strlen(lokacija));
						int zadnji = 0;
						strncpy(lokacija, &buffer[prvi+1], drugi-prvi-1);
							
							
							puts(lokacija);
							
							
							int prov = 0;
						
							if (strstr(lokacija,"/")){
								prov++;
								
								
								
								}
						
							
						if(prov > 0){
						
							logger(FORBIDDEN,"Directory not accessable.",buffer,connfd);
							
							
							}
							
							
							
							
						int buflen = strlen(lokacija);
						char fstr[12];
						int ima= 0;
						for ( int j =0;extensions[j].ext != 0;++j){
							int len = strlen(extensions[j].ext);
							if (strstr(lokacija, extensions[j].ext)){
								strcpy(fstr , extensions[j].filetype);
								ima = 1;
								break;
								}
							}
							
							
							if(ima == 0) logger(FORBIDDEN,"file extension type not supported", buffer, connfd);
							int file_fd;
							
							
						if(strcmp(lokacija,"file.txt") == 0)puts("ok");
							if ((file_fd = open(lokacija, O_RDONLY)) == -1){
								puts("Nema me");
								logger(NOTFOUND, "failed to open file",&buffer[5],connfd);
								}
							logger(LOG,"SEND",&buffer[5],0);
							
							len = (long)lseek(file_fd, (off_t)0, SEEK_END);
							(void)lseek(file_fd,(off_t)0,SEEK_SET);
							(void)sprintf(buffer,"HTTP/1.1 200 OK\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", len, fstr);
							logger(LOG,"Header",buffer,0);
							puts(buffer);
							(void)write(connfd,buffer,strlen(buffer));
							
							int ret;
							char temp[BUFSIZE];
							while((ret=read(file_fd,temp,BUFSIZE))>0){
								write(connfd,temp,ret);
								}
							
					sleep(1);
					close(connfd);
					exit(1);
                    
                    
                    }


                    if (skip == 0) {
                        char buf[1024];
                        (void) sprintf(buffer,
                                       "HTTP/1.1 200 OK\r\n");

                        strcat(buffer, "Content-Type: text/html\r\n");

                        strcat(buffer, "Content-Length: ");
                        sprintf(buf, "<!DOCTYPE html>\n<html>\n<body>\n\n");
                        strcat(buf, "\n</body>\n</html>\r\n\r\n");
                        puts(buffer);
                        puts(buf);
                        char hold[5];
                        sprintf(hold, "%d", strlen(buf));
                        int toSendSize = strlen(buffer);
                        int sent = 0;
                        strcat(buffer, hold);
                        strcat(buffer, "\r\n");
                        strcat(buffer, "Connection: close\r\n\r\n");
                        write(connfd, buffer, strlen(buffer));
                        write(connfd, buf, strlen(buf));

                    }


                    close(connfd);

                    if (send == 1) {
                        for (int i = 0; i < spaces_left; ++i) {
                            sendto(udp_socket, &msg, sizeof(msg), 0, (struct sockaddr *) &clients[i],
                                   sizeof(clients[i]));
                            char *ip = inet_ntoa(clients[i].sin_addr);
                            puts(ip);

                        }
                    }


                } else {
					(void) sprintf(buffer,
                                       "HTTP/1.1 405 Method Not Allowed\r\n");
                    write(connfd, buffer, strlen(buffer));
                    puts("nije podrzano");
                    puts(buffer);
                }

                close(connfd);
                exit(0);
            }

        }


        if (FD_ISSET(STDIN, &rset)) {
            fgets(naredba, 256, stdin);
            int send = 1;
            int izadi = 0;
            if (strcmp(naredba, "pt\n") == 0) {
                memset(&msg, 0, sizeof(msg));
                msg.command = '1';
                strcpy(msg.ipp[0].IP, "10.0.0.20");
                strcpy(msg.ipp[0].PORT, "1234");
            } else if (strcmp(naredba, "ptl\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '1';
                strcpy(msg.ipp[0].IP, "127.0.0.1");
                strcpy(msg.ipp[0].PORT, "1234");
            } else if (strcmp(naredba, "pu\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '2';
                strcpy(msg.ipp[0].IP, "10.0.0.20");
                strcpy(msg.ipp[0].PORT, "1234");
            } else if (strcmp(naredba, "pul\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '2';
                strcpy(msg.ipp[0].IP, "127.0.0.1");
                strcpy(msg.ipp[0].PORT, "1234");

            } else if (strcmp(naredba, "r\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '3';
                strcpy(msg.ipp[0].IP, "127.0.0.1");
                strcpy(msg.ipp[0].PORT, "vat");
                strcpy(msg.ipp[1].IP, "localhost");
                strcpy(msg.ipp[1].PORT, "6789");
            } else if (strcmp(naredba, "r2\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '3';
                strcpy(msg.ipp[0].IP, "20.0.0.11");
                strcpy(msg.ipp[0].PORT, "1111");
                strcpy(msg.ipp[1].IP, "20.0.0.12");
                strcpy(msg.ipp[1].PORT, "2222");
                strcpy(msg.ipp[2].IP, "20.0.0.13");
                strcpy(msg.ipp[2].PORT, "dec-notes");
            } else if (strcmp(naredba, "s\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '4';

            } else if (strcmp(naredba, "l\n") == 0) {
                for (int i = 0; i < spaces_left; ++i) {

                    char *ip = inet_ntoa(clients[i].sin_addr);
                    puts(ip);

                }


            } else if (strcmp(naredba, "n\n") == 0) {
                send = 0;
                for (int i = 0; i < spaces_left; ++i) {

                    sendto(udp_socket, "nepoznata", sizeof("nepoznata"), 0, (struct sockaddr *) &clients[i],
                           sizeof(clients[i]));
                    char *ip = inet_ntoa(clients[i].sin_addr);
                    puts(ip);

                }

            } else if (strcmp(naredba, "q\n") == 0) {

                memset(&msg, 0, sizeof(msg));
                msg.command = '0';
                puts("Kraj programa");
                izadi = 1;
                

            } else if (strcmp(naredba, "h\n") == 0) {
                puts("Ovo je pomoc");
                send = 0;
            } else {
                puts("Pritisni h za pomoc");
                send = 0;
            }

            if (send == 1) {
                for (int i = 0; i < spaces_left; ++i) {
                    sendto(udp_socket, &msg, sizeof(msg), 0, (struct sockaddr *) &clients[i],
                           sizeof(clients[i]));
                    char *ip = inet_ntoa(clients[i].sin_addr);
                    puts(ip);

                }
            }
            if(izadi == 1)
            {
				exit (0);}


        }

        if (FD_ISSET(udp_socket, &rset)) {
            int len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            int n = recvfrom(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &cliaddr, &len);

            if (strcmp(buffer, content) == 0) {
                char *ip = inet_ntoa(cliaddr.sin_addr);
                puts(ip);
                if (spaces_left < client_number) {
                    clients[spaces_left] = cliaddr;
                    spaces_left++;

                } else {
                    puts("No more space left...");

                }
            }
        }
    }


}
