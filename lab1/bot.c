#include <stdio.h>
#include <stdlib.h>
#include  <getopt.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLEN 256

void print_usage(){
	
	printf("Enter two arguments server_ip and server_port\n");
	exit(1);
	}


int main (int argc, char **argv){
	
	
	int opt;
	char *server_ip, *server_port;
	char *content = "REG\n";
	
	
	while ((opt = getopt(argc, argv, "")) != -1) { //ovdje citam argumente sto se posalju pri pozivu programa
		switch(opt)
		
		printf("There are no options avalaible");
		exit(1);
		break;
		
		
		}
	
	
	if(argc - optind !=2){
		
		
		print_usage();
		
		
		}
		
		
		server_ip = argv[optind];
		server_port = argv[optind+1];
	//tu zavrsava citanje argumenata
		struct addrinfo hints;
		//stvaranje adreses
		
		
		memset(&hints,0,sizeof(hints));
		hints.ai_family=AF_UNSPEC;
		hints.ai_protocol=0;
		hints.ai_socktype=SOCK_DGRAM;
		hints.ai_flags=AI_ADDRCONFIG|AI_PASSIVE;
		struct addrinfo* res=0;
		
		
		
		int err = getaddrinfo(server_ip,server_port,&hints,&res);
		if(err!=0){
			
			printf("failedto resolve remote socket addres (err=%d)",err);
			exit(1);
			}
		
		
		
		int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if( fd == -1) {
			printf("create socket errror\n");
			exit(1);
			}
		
		
		if(sendto(fd,content,sizeof(content), 0, res->ai_addr, res->ai_addrlen)==-1){//sljanje poruke na adresu koja se zada i na port zadani, u content je ono REG\n sto se treba poslati
			printf("send message error\n");
			exit(1);
			}
			
			
			int size =INET_ADDRSTRLEN*20+22*20+sizeof(char);
			char  buffer[size+1];//mjesto na kojem primam povratnu poruku
			struct sockaddr_storage src_addr;
			socklen_t src_addr_len=sizeof(src_addr);
			buffer[0]='\0';
			
	//cekanje i citanje poruke
			struct {
				char command;
				char IP1[INET_ADDRSTRLEN];
				char PORT1[22];
				char IP2[INET_ADDRSTRLEN];
				char PORT2[22];
				char IP3[INET_ADDRSTRLEN];
				char PORT3[22];
				char IP4[INET_ADDRSTRLEN];
				char PORT4[22];
				char IP5[INET_ADDRSTRLEN];
				char PORT5[22];
				char IP6[INET_ADDRSTRLEN];
				char PORT6[22];
				char IP7[INET_ADDRSTRLEN];
				char PORT7[22];
				char IP8[INET_ADDRSTRLEN];
				char PORT8[22];
				char IP9[INET_ADDRSTRLEN];
				char PORT9[22];
				char IP10[INET_ADDRSTRLEN];
				char PORT10[22];
				char IP11[INET_ADDRSTRLEN];
				char PORT11[22];
				char IP12[INET_ADDRSTRLEN];
				char PORT12[22];
				char IP13[INET_ADDRSTRLEN];
				char PORT13[22];
				char IP14[INET_ADDRSTRLEN];
				char PORT14[22];
				char IP15[INET_ADDRSTRLEN];
				char PORT15[22];
				char IP16[INET_ADDRSTRLEN];
				char PORT16[22];
				char IP17[INET_ADDRSTRLEN];
				char PORT17[22];
				char IP18[INET_ADDRSTRLEN];
				char PORT18[22];
				char IP19[INET_ADDRSTRLEN];
				char PORT19[22];
				char IP20[INET_ADDRSTRLEN];
				char PORT20[22];
				
				} MSG;
				ssize_t count;
				char message[512];
		
		
			while(1){
		
				printf("");
		
			count=recvfrom(fd,buffer,sizeof(buffer),0,(struct sockaddr*)&src_addr,&src_addr_len); 		
			
			if(count==-1){
			//	printf("%d",sterror(errno));
		
				}
		
		
			else if (count==sizeof(buffer)){
				printf("datagram is too large for buffer: truncated");
		
				} 
				
				
				memcpy(&MSG,&buffer,sizeof(buffer));
	
	
			if(MSG.command=='0'){
				
				char *port=MSG.PORT1;
				char *ip=MSG.IP1;
				
			int err1 = getaddrinfo(ip,port,&hints,&res);
		
			if(err1!=0){
			
				printf("failedto resolve remote socket addres (err=%d)",err);
				exit(1);
				
				}
		
		
		char hello[5] ="HELLO";
		int fd1 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		
		if( fd1 == -1) {
			printf("create socket errror\n");
			exit(1);
			}
		
		if(sendto(fd1,hello,sizeof(hello)+1, 0, res->ai_addr, res->ai_addrlen)==-1){			
			
			printf("send message error\n");
			exit(1);
			
			
			}
				
				
				struct sockaddr_storage src_addr1;
				socklen_t src_addr_len1=sizeof(src_addr1);
				memset(&message[0],0,sizeof(message));


				recvfrom(fd1,message,sizeof(message),0,(struct sockaddr*)&src_addr1,&src_addr_len1);
				printf("%s",message);
			
				
				}
				
				else if (MSG.command=='1'){
					
					
					
						
						printf("");
						
						
						char port1[20][INET_ADDRSTRLEN];
						char ip1[20][22];
				
			strcpy(port1[0],MSG.PORT1);
			strcpy(ip1[0],MSG.IP1);
			strcpy(port1[1],MSG.PORT2);
			strcpy(ip1[1],MSG.IP2);
			strcpy(port1[2],MSG.PORT3);
			strcpy(ip1[2],MSG.IP3);
			strcpy(port1[3],MSG.PORT4);
			strcpy(ip1[3],MSG.IP4);
			strcpy(port1[4],MSG.PORT5);
			strcpy(ip1[4],MSG.IP5);
			strcpy(port1[5],MSG.PORT6);
			strcpy(ip1[5],MSG.IP6);
			strcpy(port1[6],MSG.PORT7);
			strcpy(ip1[6],MSG.IP7);
			strcpy(port1[7],MSG.PORT8);
			strcpy(ip1[7],MSG.IP8);
			strcpy(port1[8],MSG.PORT9);
			strcpy(ip1[8],MSG.IP9);
			strcpy(port1[9],MSG.PORT10);
			strcpy(ip1[9],MSG.IP10);
			strcpy(port1[10],MSG.PORT11);
			strcpy(ip1[10],MSG.IP11);
			strcpy(port1[11],MSG.PORT12);
			strcpy(ip1[11],MSG.IP12);
			strcpy(port1[12],MSG.PORT13);
			strcpy(ip1[12],MSG.IP13);
			strcpy(port1[13],MSG.PORT14);
			strcpy(ip1[13],MSG.IP14);
			strcpy(port1[14],MSG.PORT15);
			strcpy(ip1[14],MSG.IP15);
			strcpy(port1[15],MSG.PORT16);
			strcpy(ip1[15],MSG.IP16);
			strcpy(port1[16],MSG.PORT17);
			strcpy(ip1[16],MSG.IP17);
			strcpy(port1[17],MSG.PORT18);
			strcpy(ip1[17],MSG.IP18);
			strcpy(port1[18],MSG.PORT19);
			strcpy(ip1[18],MSG.IP19);
			strcpy(port1[19],MSG.PORT20);
			strcpy(ip1[19],MSG.IP20);



			for(int i=0;i<15;++i){
			for(int j=0;j<20;++j){
			int err1 = getaddrinfo(ip1[j],port1[j],&hints,&res);
		
			if(err1!=0){
			
				continue;
				exit(1);
				
				}
		
		int fd2 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if( fd2 == -1) {

			continue;
			exit(1);

			}



		printf("");
		
			printf("%d",i);
		
		printf("\n");



			int setZero=0;



				for(int k=0;k<sizeof(message);++k){
		
						if(message[k]=='\0'){
											setZero=1;
											}
			
					if(setZero){
							
							message[k]='\0';
				
				}
		}
		
		
		sendto(fd2,message,sizeof(message), 0, res->ai_addr, res->ai_addrlen);
					printf("\n");	
				
					
					
				}
				sleep(1);
				}
					
					}
					
					else {
						
						exit(0);
						
	
}}
	
	}
