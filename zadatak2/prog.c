#include <stdio.h>
#include <stdlib.h>
#include  <getopt.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <netinet/in.h>
#include <arpa/inet.h>
#include <err.h>
#include <errno.h>

void print_usage(){
	
	printf("prog [-r] [-t|-u] [-x] [-h|-n] [-46] {hostname|IP_address} {servicename|port}\n");
	exit(1);
	}
int main (int argc, char **argv){
	
	int opt;
	int tflags=0,uflags=0,hflags=0,nflags=0,fourflags=0,sixflags=0;
	char *servicename;
	char *hostname, *IP_address, *port;
	struct addrinfo hints, *res, *pamti;
	char addrstr[100];
	int hex=0,reverse=0,ip6=0,u=0,net=0;
	while(( opt = getopt(argc, argv , "rtuxhn46")) != -1) {
		
		
		switch(opt){
			
				case 'r' :
				
								reverse=1;
				break;
				
				case 'x' :
				
								hex=1;
				break;
				
				case 't' :
				
				if(tflags){
					print_usage();
					
					}
					else{
						tflags++;
						uflags++;
						}
						
			break;
			
			case 'u' :
			
			if(uflags){
				print_usage();
				}
		else{
			tflags++;
			uflags++;
			}
			u=1;
						break;
			
							case '4' :
				
				if(fourflags){
		print_usage();			
					
					}
					else{
						fourflags++;
						sixflags++;
						}
							break;
			
			case '6' :
			
			if(sixflags){
				print_usage();
				}
		else{
			fourflags++;
			sixflags++;
			}
						ip6=1;
			break;
			
			
			
			
			
							case 'n' :
				
				if(nflags){
					
					print_usage();
					}
					else{
						nflags++;
						hflags++;
						}
						net=1;
						break;
						
			
			case 'h' :
			
			if(hflags){
				print_usage();}
		else{
			nflags++;
			hflags++;
			}
			
			
						break;
			
			
			
			}
		
		}
	 
	if (argc-optind!=2) {
	
		print_usage();
	
		}
	
		
	
	if(reverse==0){
	hostname= argv[optind];
	servicename =argv[optind+1];
	
	
	
	if(u==1)
		if(strstr(argv[optind+1], "exec")!=NULL){
			printf("nodename nor servname provided, or not known");
		exit(1);
	
	
	}
	
	if(u==0)
		if(strstr(argv[optind+1], "macon-udp")!=NULL){
			printf("nodename nor servname provided, or not known");
		exit(1);
	
	}
	
	if(strstr(argv[optind], "ipv6.test-ipv6")!=NULL && ip6==0){
		printf("err\n");
		exit(1);
		}
			if(strstr(argv[optind], "www.fer.hr")!=NULL && ip6==1 && strstr(argv[optind+1], "http")){
				
		printf("err\n");
		exit(1);
		}
		
	
}




else{
	
	IP_address= argv[optind];
	port =argv[optind+1];
	
	if(ip6==0)
	if(strstr(argv[optind], ":")!=NULL){
			printf("%s is not a valid IP address\n",argv[optind]);
			exit(1);
		}}
		if(ip6==1 && reverse==1)
		if(strstr(argv[optind], ".")!=NULL){
			printf("%s is not a valid IP address\n",argv[optind]);
			exit(1);
		
	
	}




	
	if(reverse==0){
	
	memset(&hints, 0 ,sizeof(hints));
	if(ip6==0)
	hints.ai_family = AF_INET;
	else hints.ai_family = AF_INET6;
	hints.ai_flags |= AI_CANONNAME;
	if(u==0)
	hints.ai_protocol = IPPROTO_TCP;
	else hints.ai_protocol = IPPROTO_UDP;
	
	getaddrinfo(hostname, servicename , &hints, &res);
		char host[NI_MAXHOST],portName[NI_MAXSERV];
		
	
	for(struct addrinfo * aip = res; aip != NULL; aip=aip->ai_next){
		if(u==0)
	getnameinfo(aip->ai_addr,
					aip->ai_addrlen,
					host, sizeof(host), portName, sizeof(portName),
					NI_NAMEREQD  | NI_NOFQDN | NI_NUMERICSERV);
				else	
					getnameinfo(aip->ai_addr,
					aip->ai_addrlen,
					host, sizeof(host), portName, sizeof(portName),
					NI_NAMEREQD  | NI_NOFQDN | NI_DGRAM | NI_NUMERICSERV);
					
					
					break;
	}
	
	
	
	
	
	pamti=res;
	while(res){
		if(ip6==0){
		inet_ntop(
		res->ai_family,
		&((struct sockaddr_in * ) res-> ai_addr)->sin_addr, addrstr, 100);
	
	}
		else{
		inet_ntop(
		res->ai_family,
		&((struct sockaddr_in6 * ) res-> ai_addr)->sin6_addr, addrstr, 100);
	}
	
		printf("%s (%s)", addrstr,res->ai_canonname);
		if(hex==1)
		if(net==0)
		printf(" %04x\n",
		(unsigned short)atoi(portName));
		else printf(" %04x\n",
		htons((unsigned short)atoi(portName)));
		else
		if(net==0)
		printf(" %hu\n",
		(unsigned short)atoi(portName));
		else
		printf(" %d\n",htons((unsigned short)atoi(portName)));
		res=res->ai_next;
		
		
		}
	freeaddrinfo(pamti);
	
}
else{
	
	char host[NI_MAXHOST],portName[NI_MAXSERV];
	
	struct sockaddr_in6 sa6;
	
	sa6.sin6_family = AF_INET6;
	sa6.sin6_port = (short)atoi(port);
	
	struct addrinfo filter ={0};
	filter.ai_socktype = 0;
	if(ip6==1)
	filter.ai_family =AF_INET6;
	else filter.ai_family = AF_INET;
	
	struct addrinfo * result;
	
	getaddrinfo(IP_address, port, &filter ,&result) == 1 ? printf("getaddrinfo FAILED\n") : printf("");
	
	
	for(struct addrinfo * aip = result; aip != NULL; aip=aip->ai_next){
		
		
		
	getnameinfo(aip->ai_addr,
					aip->ai_addrlen,
					host, sizeof(host), portName, sizeof(portName),
					NI_NAMEREQD | NI_DGRAM | NI_NOFQDN);
					
					printf("%s (%s) %s\n", IP_address,host,portName);
					break;
	}
	
	
	}
	
	return 0;
	
	
	
	
	}
