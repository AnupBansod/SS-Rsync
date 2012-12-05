#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
//#include "aamche_header.h"

int main(int argc , char **argv )
{
     //extern int aamche_portno;
     int sockfd,portno;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
   
     
	
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(atoi(argv[1]));
     

     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	printf("\n\nerror while binding the socket"); 
              


     int no=listen(sockfd,5);
     if(no == 0)
	printf("\n\n server is in listening mode on port no %d",atoi(argv[1]));     

	FILE *fp;
	fp=fopen("testingsocket.txt","w");
	fprintf(fp,"listen has been executed");

   return 0; 
}
