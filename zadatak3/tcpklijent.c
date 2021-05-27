#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define MAXDATASIZE 100000

int
main(int argc, char *argv[])
{
	

	int opt;
    unsigned int PORT=3490;
	char *server="127.0.0.1";
	int c=0;
	int count=0;


				while((opt=getopt(argc,argv,"s:p:c")) != -1){
					
					
					switch(opt){
			case 'c':
			c=1;
			count++;
			break;			
						
						case 'p': 
					count+=2;
									PORT = (unsigned int)atoi(optarg);
									break;
						
						case 's':
							count+=2;
							server=optarg;	
								break;
								
						default: printf("Kriva opcija\n"); return 1;
						
							
						}
					
					
					}
	int offset=0;	
	    
    FILE *filename;
	int test = access(argv[argc-1],F_OK);
	if(test==0 && c==0)
			{
				err(3,"greska1\n");
				exit(3);
			}
			
	else if(test<0 ){
		filename=fopen(argv[argc-1],"w+");
				if(filename==NULL){
					err(3,"greska2");
					exit(3);
					}
					fclose(filename);
					filename=fopen(argv[argc-1],"r+");
					if(filename==NULL)exit(3);
		}
	
	else if(test==0 && c==1){
			filename=fopen(argv[argc-1],"a+");
					if(filename==NULL)
					exit(3);
				}
				
	else if(test==0 && c==0){
		filename=fopen(argv[argc-1],"w+");
				if(filename==NULL){
						err(3,"greska3");
						exit(3);}
					}
					
test = access(argv[argc-1],W_OK);

	if(test<0 && c==1)
	{err(3,"greska4\n");exit(3);}
	
    
	
	
    int		    sockfd, numbytes;
    char	    buf    [MAXDATASIZE];
    struct sockaddr_in their_addr;
    int error;
    struct addrinfo hints, *res;
if(argc-count==1){printf("nije unesena datoteka\n");exit(1);}
	char *sendFile=argv[argc-1];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags |= AI_CANONNAME;

    if ((error=getaddrinfo(server, NULL, &hints, &res))) 
        errx(1, "%s", gai_strerror(error));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	err(1,"socket");
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if (connect(sockfd,(struct sockaddr *)&their_addr,sizeof their_addr) == -1) {
	err(1,"connect");
    }
    
    char z;
    FILE *filename1=fopen(argv[argc-1],"r");
    if(filename1==NULL)err(7,"prob");
    while(1){
		z=fgetc(filename1);
		
		if(z==EOF)break;
		offset++;
		}
    fclose(filename1);
    char off[11];
    snprintf(off,sizeof(off),"%d",offset);
    snprintf(off, sizeof(off), "%09d", offset);
    //printf("%s",off);
    char m[strlen(sendFile)+strlen(off)+2];
    snprintf(m,sizeof(m),"%s%s",off,sendFile);
    //printf("%s",m);
     if (send(sockfd, m, strlen(m)+10, 0) == -1)
        perror("send");

    
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE+10000 - 1, 0)) == -1) {
	err(1,"recv");
    }
    buf[numbytes] = '\0';
    char exitStatus[5];
	strncpy(exitStatus,buf,4);
	exitStatus[4]='\0';
	char toPut[strlen(buf)];
	strncpy(toPut,buf+4,strlen(buf));
   // printf("%s\n",exitStatus);
   printf("\nReceived: %s\n", toPut);
  
    

    
    
    
    
    if(strstr(exitStatus,"1")){fclose(filename);close(sockfd);exit(1);}
    else if(strstr(exitStatus,"2")){fclose(filename);close(sockfd);exit(2);}
    else if (strstr(exitStatus,"3")){fclose(filename);close(sockfd);exit(3);}
    else{
		
	
		
		}
		
		
		fseeko(filename,-1,SEEK_END);
		int position=ftell(filename);
		ftruncate(fileno(filename), position);
		
		
	fwrite(toPut,1,strlen(toPut),filename);
   fclose(filename);

    close(sockfd);

    return 0;
}
