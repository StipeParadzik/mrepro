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
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#define RECV_TIMEOUT 4
#define RECV_RETRIES 4

int daemon_init () {
pid_t pid;
pid= fork();
if(pid<0)exit(EXIT_FAILURE);
if(pid>0)exit(EXIT_SUCCESS);
if(setsid()<0)exit(EXIT_FAILURE);

signal(SIGCHLD,SIG_IGN);
signal(SIGHUP,SIG_IGN);

pid = fork();

if(pid<0)exit(EXIT_FAILURE);

if(pid>0)exit(EXIT_SUCCESS);

umask(0);


int x;
for(x=sysconf(_SC_OPEN_MAX);x>=0;x--){
	close(x);
	}

openlog("bakugan",LOG_PID,LOG_DAEMON);
}


enum vrsta_naredbe {
    RRQ = 1,
    WRQ,
    DATA,
    ACK,
    ERROR
};

enum mode {
    NETASCII = 1,
    OCTET
};

typedef union {

    uint16_t vrsta_naredbe;

    struct {
        uint16_t vrsta_naredbe;
        uint8_t filename_and_mode[514];
    } request;

    struct {
        uint16_t vrsta_naredbe;
        uint16_t blok;
        uint8_t data[512];
    } data;

    struct {
        uint16_t vrsta_naredbe;
        uint16_t blok;
    } ack;

    struct {
        uint16_t vrsta_naredbe;
        uint16_t error_code;
        uint8_t error_string[512];
    } error;

} tftp_poruka;


char *direktorij;

void cld_handler(int sig) {
    int status;
    wait(&status);
}


ssize_t ack_send(int s, uint16_t blok,
                 struct sockaddr_in *sock, socklen_t sender_address_size) {
    tftp_poruka m;
    ssize_t c;

    m.vrsta_naredbe = htons(ACK);
    m.ack.blok = htons(blok);

    if ((c = sendto(s, &m, sizeof(m.ack), 0,
                    (struct sockaddr *) sock, sender_address_size)) < 0) {
        perror("server: sendto()");
    }

    return c;
}

ssize_t send_data(int s, uint16_t blok, uint8_t *data,
                  ssize_t dlen, struct sockaddr_in *sock, socklen_t sender_address_size) {
    tftp_poruka m;
    ssize_t c;

    m.vrsta_naredbe = htons(DATA);
    m.data.blok = htons(blok);
    memcpy(m.data.data, data, dlen);

    if ((c = sendto(s, &m, 4 + dlen, 0,
                    (struct sockaddr *) sock, sender_address_size)) < 0) {
        perror("send error\n");
    }

    return c;
}

ssize_t error_send(int s, int error_code, char *error_string,
                   struct sockaddr_in *sock, socklen_t sender_address_size) {
    tftp_poruka m;
    ssize_t c;

    if (strlen(error_string) >= 512) {
        fprintf(stderr, "linija preduga mora biti manja od 512\n");
        return -1;
    }

    m.vrsta_naredbe = htons(ERROR);
    m.error.error_code = error_code;
    strcpy(m.error.error_string, error_string);

    if ((c = sendto(s, &m, 4 + strlen(error_string) + 1, 0,
                    (struct sockaddr *) sock, sender_address_size)) < 0) {
        perror("sendTo error \n");
    }

    return c;
}

ssize_t tftp_recv_message(int s, tftp_poruka *poruka, struct sockaddr_in *sock, socklen_t *sender_address_size) {
    ssize_t c;

    if ((c = recvfrom(s, poruka, sizeof(*poruka), 0, (struct sockaddr *) sock, sender_address_size)) < 0
        && errno != EAGAIN) {
        perror("recvfrom error \n");
    }

    return c;
}

void tftp_handle_request(tftp_poruka *m, ssize_t len,
                         struct sockaddr_in *sender_address, socklen_t sender_address_size) {
    int s;
    struct protoent *pp;
    struct timeval tv;

    char *filename, *mode_s, *end;
    FILE *fd;

    int mode;
    uint16_t vrsta_naredbe;

   
    if ((pp = getprotobyname("udp")) == 0) {
        fprintf(stderr, "get prot erorr\n");
        exit(1);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, pp->p_proto)) == -1) {
        perror("socket server");
        exit(1);
    }

    tv.tv_sec = RECV_TIMEOUT;
    tv.tv_usec = 0;

    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("sock opt error");
        exit(1);
    }

   
    filename = m->request.filename_and_mode;
    end = &filename[len - 2 - 1];

    if (*end != '\0') {
        printf("%s.%u ime datoteke neispravno\n",
               inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
        error_send(s, 0, "neispravno ime datoteke ili opcija", sender_address, sender_address_size);
        exit(1);
    }

    mode_s = strchr(filename, '\0') + 1;

    if (mode_s > end) {
        printf("%s.%u: transfer nije specificiran\n",
               inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
        error_send(s, 0, "opcija nije specificirana", sender_address, sender_address_size);
        exit(1);
    }

    if (strncmp(filename, "../", 3) == 0 || strstr(filename, "/../") != NULL ||
        (filename[0] == '/' && strncmp(filename, direktorij, strlen(direktorij)) != 0)) {
        printf("%s.%u: filename outside base directory\n",
               inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
        error_send(s, 0, "filename outside base directory", sender_address, sender_address_size);
        exit(1);
    }

    vrsta_naredbe = ntohs(m->vrsta_naredbe);
    if (vrsta_naredbe != RRQ) {
        perror("samo RRQ\n");
        exit(12);
    }
    fd = fopen(filename, "r");

    if (fd == NULL) {
        perror("nemogu otvoriti");
        error_send(s, errno, strerror(errno), sender_address, sender_address_size);
        exit(1);
    }

    mode = strcasecmp(mode_s, "ascii") ? NETASCII :
           strcasecmp(mode_s, "octet") ? OCTET :
           0;

    if (mode == 0) {
        printf("%s.%u: nevaljana transfer opcija\n",
               inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
        error_send(s, 0, "nevaljana transfer opcija", sender_address, sender_address_size);
        exit(1);
    }

    printf("%s.%u: zaprimljen zahtijev %s '%s' %s\n",
           inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port),
           ntohs(m->vrsta_naredbe) == RRQ ? "get" : "put", filename, mode_s);


    if (vrsta_naredbe == RRQ) {
        tftp_poruka m;

        uint8_t data[512];
        ssize_t dlen, c;

        uint16_t blok = 0;

        int brojac;
        int zatvori = 0;

        while (!zatvori) {

            dlen = fread(data, 1, sizeof(data), fd);
            blok++;

            if (dlen < 512) {
                zatvori = 1;
            }

            for (brojac = RECV_RETRIES; brojac; brojac--) {

                c = send_data(s, blok, data, dlen, sender_address, sender_address_size);

                if (c < 0) {
                    printf("%s.%u: transfer zaustavljen\n",
                           inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
                    exit(1);
                }

                c = tftp_recv_message(s, &m, sender_address, &sender_address_size);

                if (c >= 0 && c < 4) {
                    printf("%s.%u: poruka sa nevaljalom velicinom\n",
                           inet_ntoa(sender_address->sin_addr), ntohs(sender_address->sin_port));
                    error_send(s, 0, "nevaljana velicina", sender_address, sender_address_size);
                    exit(1);
                }

                if (c >= 4) {
                    break;
                }

                if (errno != EAGAIN) {
                    printf("transfer prekinut\n"
                    );
                    exit(1);
                }

            }

            if (!brojac) {
                printf("predugo trajanje transfera\n"
                );
                exit(1);
            }

            if (ntohs(m.vrsta_naredbe) == ERROR) {
                printf("error\n");
                exit(1);
            }

            if (ntohs(m.vrsta_naredbe) != ACK) {
                printf("ack problem\n");
                exit(1);
            }

            if (ntohs(m.ack.blok) != blok) {
                printf("navljan ack broj\n");
                exit(1);
            }

        }

    } else if (vrsta_naredbe == WRQ) {

        perror("WRQ nije podrzan\n");
        exit(1);
    }


    fclose(fd);
    close(s);

    exit(0);
}

int main(int argc, char *argv[]) {
    int s;
    uint16_t port = 0;
    struct protoent *pp;
    struct servent *ss;
    struct sockaddr_in server_sock;
    
    
	int opt;
int d =0 ;
	
	
	while ((opt = getopt(argc, argv, "d")) != -1) { //ovdje citam argumente sto se posalju pri pozivu programa
		switch(opt)
		
	case 'd':
	
	d=1;
	
	break;
	prinf("./tftpserver [-d] port_name_or_number\n");
	exit(1);
		break;
		
		
		}
		if(d==1)
		daemon_init();
	
    if (argc < 2) {
        printf("./tfptserver [-d] port_name_or_number\n", argv[optind]);
        exit(1);
    }

    if (chdir("/tftpboot") < 0) {
        perror("nevljan direktorij");
        exit(1);
    }

    if (argc > 1) {
        if (sscanf(argv[1], "%hu", &port)) {
            port = htons(port);
        } else {
            fprintf(stderr, "nevljan port\n");
            exit(1);
        }
    } else {
        if ((ss = getservbyname("tftp", "udp")) == 0) {
            fprintf(stderr, "ime ne mogu pronoaci\n");
            exit(1);
        }

    }

    if ((pp = getprotobyname("udp")) == 0) {
        fprintf(stderr, "ime nemogu pornaci\n");
        exit(1);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, pp->p_proto)) == -1) {
        perror("socket error");
        exit(1);
    }

    server_sock.sin_family = AF_INET;
    server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sock.sin_port = port ? port : ss->s_port;

    if (bind(s, (struct sockaddr *) &server_sock, sizeof(server_sock)) == -1) {
        perror("bind error");
        close(s);
        exit(1);
    }

    signal(SIGCHLD, (void *) cld_handler);

    while (1) {
        struct sockaddr_in sender_address;
        socklen_t sender_address_size = sizeof(sender_address);
        ssize_t len;

        tftp_poruka message;
        uint16_t vrsta_naredbe;

        if ((len = tftp_recv_message(s, &message, &sender_address, &sender_address_size)) < 0) {
            continue;
        }

        if (len < 4) {
            printf("%s.%u: nevaljana velicina\n",
                   inet_ntoa(sender_address.sin_addr), ntohs(sender_address.sin_port));
            error_send(s, 0, "invalid nevaljana velicnan", &sender_address, sender_address_size);
            continue;
        }

        vrsta_naredbe = ntohs(message.vrsta_naredbe);

        if (vrsta_naredbe == RRQ) {


            if (fork() == 0) {
                tftp_handle_request(&message, len, &sender_address, sender_address_size);
                exit(0);
            }

        } else {
            printf("nevaljan zahtijev: vrsta_naredbe \n");
            error_send(s, 0, "invalid vrsta_naredbe", &sender_address, sender_address_size);
            exit(1);
        }

    }

    close(s);

    return 0;
}
