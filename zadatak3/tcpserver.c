#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define BACKLOG 10

int 
main(int argc, char **argv)
{
	
	
	int opt;
    unsigned int port=3490;
	int count=0;


				while((opt=getopt(argc,argv,"p:")) != -1){
					
					
					switch(opt){
						
						
						case 'p': 
									count+=2;
									
									for(int i=0;i<strlen(optarg);++i){
										if(isalpha(optarg[i])){err(1,"port");}
										}
									port = (unsigned int)atoi(optarg);
									break;
						
									
						default: printf("Kriva opcija\n"); return 1;
						
							
						}
					
					
					}
	if(argc-count!=1){printf("nevaljan unso\n");exit(1);}
	
	
	
	while(1){
    int sockfd, newfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    int error;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		err(1,"socket");
    }
    int option=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
		err(1,"bind");
    }

    if (listen(sockfd, BACKLOG) == -1) {
		err(1,"listen");
    }

    sin_size = sizeof their_addr;
    if ((newfd=accept(sockfd,(struct sockaddr *)&their_addr,&sin_size)) == -1) {
        err(1,"accept");
    }

    if ( (error = getnameinfo((struct sockaddr *)&their_addr, sin_size,
                              hbuf, sizeof(hbuf), sbuf, sizeof(sbuf),
                              NI_NOFQDN | NI_NUMERICSERV))) {
        errx(1, "%s", gai_strerror(error));
    }
    
    
//-------------------------------------------zavrsava postavljanje    
	char message[1000];
	int numbytes;
	if ((numbytes = recv(newfd, message, sizeof(message), 0)) == -1) {
	err(1,"recv");
    }
    message[numbytes]='/0';
	//printf("%s\n",message);
    
   char offset[10];
   char fileName[995];
   strncpy(offset,message,9);
   strncpy(fileName,message+9,995);
   //printf("%s\n",fileName);
   //printf("%s\n",offset);
  

int test;

//if(strstr(fileName,"/")==NULL)test=-1;


  test=access(fileName,R_OK);

 
 	 
	FILE *f=fopen(fileName,"r+");
	fseek(f,0,SEEK_END);
	long fsize= ftell(f);
	int broj = atol(offset);
	
	
	 char z;int isIt=0;
    FILE *filename1=fopen(fileName,"r+");
    if(filename1==NULL)err(7,"prob");
    while(1){
		z=fgetc(filename1);
		
		if(z==EOF)break;
		isIt++;
		}
    fclose(filename1);
	if(broj>0)broj--;
	printf("%d %d\n",broj,isIt);
	if(isIt<=broj+1)broj=0;
	
	
	
	
	//printf("1231232   %d\n",broj);
	fseek(f, broj, SEEK_SET);
	
	char *string = malloc(fsize+1);
	fread(string, 1, fsize, f);

	string[fsize]=0;
	 
	//printf("%s\n\n\n",string);
	 

 
 char *sendBack;
 if(test==0){
	if(f!=NULL)
	 sendBack="0x00";
	 else sendBack="0x02";
	 char *result=malloc(strlen(sendBack)+strlen(string)+1);
	 strcpy(result,sendBack);
	strcat(result,string);
	//printf("%s",result);
	
	// printf("%s\n",result);
    //printf("server: got connection from %s:%s\n", hbuf,sbuf);
    if (send(newfd, result,strlen(result)+1, 0) == -1)
        perror("send");
     	fclose(f);  

    close(newfd);

    close(sockfd);
	
	 }
 else if (test<0){ 
	 
	 
	 sendBack="0x01datoteka ne postoji";
	 if (send(newfd, sendBack, 30, 0) == -1)
        perror("send");
        close(newfd);
        continue;
	 
	 }
 
 
 
 else {
	 sendBack="0x03GRESKA";
	 if (send(newfd, sendBack, 14, 0) == -1)
        perror("send");
        close(newfd);
        continue;
	 
	
	 }
 

}

    return 0;
}
