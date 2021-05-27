#include <stdio.h>
#include <stdlib.h>
#include  <getopt.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 256

void print_usage(){
	
	printf("Enter two arguments server_ip and server_port\n");
	exit(1);
	}


int main (int argc, char **argv){
	

	int opt;
	char *port="1234", payload[500]="";
	
	int count=0;
	int p=0;
	int l=0;
	
	while ((opt = getopt(argc, argv, "p:l:")) != -1) { //ovdje citam argumente sto se posalju pri pozivu programa
		switch(opt){
		
		case 'l':
		if(count==0){l=1;count++;}
			else {count++;l=2;}
		break;
		case 'p':
		if(count==0){count++;p=1;}
		else {count++; p=2;}
		
		break;
		
		printf("There are no options avalaible");
		exit(1);
		break;
	}
		
		}
	
	
	
	printf("%d %d \n",p,l);
			if(l==1){
				
	//				printf("payload = %s\n",argv[optind-3]);
				
				port=argv[optind-3];
				
				if(p==2){
		//			printf("port = %s\n",argv[optind-1]);
			strcpy(payload,argv[optind-1]);
					}
				}
				else if(l==0){
					
					if(p==1){
			//								printf("port = %s\n",argv[optind-1]);

						//payload=argv[optind-1];
						strcpy(payload,argv[optind-1]);
						}
					
					}
					
					else if(l==2){
						
				//							printf(" payload = %s\n",argv[optind-1]);
					port=argv[optind-1];
//					printf("port = %s\n",argv[optind-3]);
				strcpy(payload,argv[optind-3]);
						
						}

				//char* hostname="10.0.2.15";
				char* hostname="127.0.0.1";
				struct addrinfo hints;
				memset(&hints,0,sizeof(hints));
				hints.ai_family=AF_UNSPEC;
				hints.ai_socktype=SOCK_DGRAM;
				hints.ai_protocol=0;
				hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
				struct addrinfo* res=0;
				int err=getaddrinfo(hostname,port,&hints,&res);
				if(err!=0){
					printf("getaddrinfo error\n");
					exit(1);
					}
				
			int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
			if(fd==-1){
				printf("socket error\n");
				exit(1);
				}

		if(bind(fd,res->ai_addr,res->ai_addrlen)==-1){
			printf("bind error\n");
			exit(1);
				}

		freeaddrinfo(res);

while(1){
		char buffer[10];
		struct sockaddr_in cliaddr;
		socklen_t clilen;
		clilen=sizeof(cliaddr);
		ssize_t counter=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr *) &cliaddr,&clilen);
		if(counter==-1){
			printf("error");
			exit(1);
			}
			else if (count==sizeof(buffer)){
				printf("datagram too large for buffer\n");
				}
				else {
				
					}
				
			
				
				if(strcmp(buffer,"HELLO")==0){
					printf("tu sam\n");
						char send[512];
						memset(&send[0],0,sizeof(send));
						strcpy(send,"PAYLOAD:");
						strcat(send,payload);
						strcat(send,"\n");
					ssize_t counter2=sendto(fd,send,sizeof(send),0,(struct sockaddr*)&cliaddr,clilen);
				if(counter2==-1)printf("send error");
	
}
	
					}

}
