/* SUPERNODE FILE */

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include<arpa/inet.h>

char buffer[1024];
char buffer1[1024];

int i=0,j;
char * registration(char buffer[1024]);
char * connectname(char buffer[1024]);
void *check_other_supernode(void*);
char * searchnode(char * uname);
char uname1[20][20],ipaddr1[20][20],port1[20][20];
char *temp1,*temp2,temp3;


int main( int argc, char *argv[] ) 
{

	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int  n;
	char * token, *token1, uname[20],ipaddr[50],port[10];
	int totalsupernode,k;
	char supernodeip[10];
	pthread_t bthread;
	pthread_create(&bthread,NULL,&check_other_supernode,NULL);
    
	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
	perror("ERROR opening socket\n");
	exit(1);
	}

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	perror("ERROR on binding\n");
	exit(1);
	}
	printf("1");

	/* Now start listening for the clients, here process will
	* go in sleep mode and will wait for the incoming connection
	*/

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	setbuf(stdout,NULL);
    
	while(1)
	{
		//printf("1...\n");
		/* Accept actual connection from the client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		printf("1");
		if (newsockfd < 0) {
			perror("ERROR on accept\n");
			exit(1);
			}
		printf("12\n");
		bzero(buffer,1024);
		n = read( newsockfd,buffer,255 );
		printf("reading buffer %s",buffer);
		if (n < 0) {
			perror("ERROR reading from socket\n");
			exit(1);
			}
		token = strtok(buffer,"#");
		printf("token outside %s\n",token);
		if(strcmp(token,"reg")== 0)
		{
			printf("about to register\n");
			temp1 = registration(token);
		n = write(newsockfd,"I got your message",18);

		if (n < 0)
        {
			perror("ERROR writing to socket\n");
			exit(1);
			}
		}
		printf("\n Before conn\n");
		if(strcmp(token,"conn")==0)
		{
			printf("about ot coonect\n");
			bzero((char *)&buffer1,sizeof(buffer1));
			sprintf(buffer1, "%s", connectname(token));
			printf("value in temp2: %s\n",buffer1);
            
	 		/* Write a response to the client */
			n = write(newsockfd,buffer1,sizeof(buffer1));
		}
		

		if (n < 0)
        {
			perror("ERROR writing to socket\n");
			exit(1);
        }

		/* If connection is established then start communicating */


	}
close(sockfd);

return 0;
}

//FUNCTION FOR THE REGISTRATION ON SUPERNODE BY USER

char * registration(char * token)
{
	char * token1, uname[20],ipaddr[50],port[10];
	int serv_sockfd;
	printf("buffer ---> %s\n",token);
  	memset(uname,0,20);
	memset(ipaddr,0,50);
	memset(port,0,10);
	token1 = strtok(NULL,"#");
	strcpy(uname,token1);
	strcpy(uname1[i],uname);
	printf("Here is the message- username = %s\n",uname);
	token1 = strtok(NULL,"#");
	strcpy(ipaddr, token1);
	strcpy(ipaddr1[i],ipaddr);
   	printf("Here is the message:-ipaddress = %s\n",ipaddr);
	token1 = strtok(NULL,"#");
	strcpy(port, token1);
	strcpy(port1[i],port);
   	printf("Here is the message:-port number = %s\n",port);
   	for(j=0;j<=i;j++)
	{
	printf("%d,%s,%s,%s\n",j,uname1[j],ipaddr1[j],port1[j]);
	}
	i++;
  
return "0";
}

//FUNCTION TO FIND A USERNAME FOUND IN THE ENTRIES OF ONE SUPERNODE THEN RETURN ITS IP OR ASK ANOTHER SUPERNODE WHICH DOES THE SAME THING

char * connectname(char * token)
{
	char * token1, uname[20];
	char fname[20],fipaddr[20],fport[20];
	char * sendbuff = malloc(500);
	char sBuffer[1024];
	char recvBuff[1024];
	int user_not_found = 0,portno2;
	int user_found = 0;
	char * supernodeip = malloc(20);
	int serv_sockfd,ret;
	struct sockaddr_in serv_addr, cli_addr;
	printf("buffer ---> %s\n",token);
	token1 = strtok(NULL,"#");
	strcpy(uname,token1);
	printf("name about to search %s\n",uname);
		
	for(j=0;j<=i;j++)
	{
	printf("%d,%s,%s,%s\n",j,uname1[j],ipaddr1[j],port1[j]);
	}
	
	int k =0;
	while(k<20)
    {
            if(strcmp(uname1[k], uname) == 0) 
			{
                printf("Gotcha %s!\n",uname);
				strcpy(fname,uname1[k]); 
				strcpy(fipaddr,ipaddr1[k]);
				strcpy(fport,port1[k]);
				printf("found enteries are :%s,%s,%s\n",fname,fipaddr,fport);
				memset( sendbuff, '\0', sizeof( sendbuff ));
				strcpy(sendbuff,fname);
				strcat(sendbuff,"#");
				strcat(sendbuff,fipaddr);
				strcat(sendbuff,"#");
				strcat(sendbuff,fport);
				user_found = 1;
                break;
	        }
		k++;
	}
	if(user_found != 1)
	{		
		printf("About to ask other supernodes\n");
		strcpy(sBuffer,"search#");
		strcat(sBuffer,uname);
		printf("sBuffer:%s\n",sBuffer);
		while(user_not_found == 0)
		{
			if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
			{
				  perror("Socket Error\n");
				  exit(0);
			}
			
			printf("Enter supernode IP:\n");
			scanf("%s",supernodeip);
			printf("Enter PORT_NO:\n");	
			scanf("%d",&portno2);
			bzero((char *)&serv_addr,sizeof(serv_addr));
			ret = inet_aton(supernodeip, &serv_addr.sin_addr);
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(portno2);

		  // Connecting to the Server
			if(connect(serv_sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
			{
				close(serv_sockfd);
				perror("Connect Error\n");
				exit(0);
			}
			
			if(write(serv_sockfd,sBuffer,sizeof(sBuffer))!= sizeof(sBuffer))
			{
				printf("write error");
				exit(0);
			}
			if(read(serv_sockfd, recvBuff, sizeof(recvBuff)) < 0)
			{
				perror("Read Error\n");
				exit(0);
			}
			printf("recvBuff:%s\n", recvBuff);
			//compare if user_not_found = 'N'
			if(strcmp(recvBuff,"N") == 0)
			{
				user_not_found = 0;
			}
			else
			{
				user_not_found = 1;
				strcpy(sendbuff,recvBuff);
				printf("sendbuff:%s\n", sendbuff);
			}
			
	
			
		}
		close(serv_sockfd);
	}
	return sendbuff;
}

//FUNCTION TO SWITCH TO ANOTHER SUPERNODE

void *check_other_supernode(void *ptr)
{
int sockfd, newsockfd, portno, clilen;
struct sockaddr_in serv_addr, cli_addr;
char * token, * uname;
char recvBuff[1024];
char * unameresult;

	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("ERROR opening socket\n");
		exit(1);
	}

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	//portno = 2626;
	
	printf("Enter the port number\n");
	scanf("%d",&portno);
	//PORT_NO = portno;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding\n");
		exit(1);
	}
	printf("Thread Running\n");

	/* Now start listening for the clients, here process will
	* go in sleep mode and will wait for the incoming connection
	*/

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	setbuf(stdout,NULL);
	
	while(1)
	{
		printf("Thread Inside while\n");
		/* Accept actual connection from the client */
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		printf("Thread after accept");
		if (newsockfd < 0) 
		{
			perror("ERROR on accept\n");
			exit(1);
		}
			
		if(read(newsockfd, recvBuff, sizeof(recvBuff)) < 0)
		{
			perror("Read Error\n");
			exit(0);
		}
		printf("Thread - the recvbuff is %s\n",recvBuff);		
		token = strtok(recvBuff,"#");
		printf("Thread - the token is %s\n",token);
		if(strcmp(token,"search") == 0)
		{
			uname = strtok(NULL,"#");
			char send_results[1024];
			sprintf(send_results, "%s", searchnode(uname));
			printf("send_results: %s\n",send_results);
						
			if(write(newsockfd,send_results,sizeof(send_results))!= sizeof(send_results))
			{
			printf("write error");
			exit(0);
			}				
		}
	}
}

//FUNCTION TO SEARCH FOR ENTRIES IN A SUPERNODE

char * searchnode(char * uname)
{

char * sendbuff = malloc(500);
int k = 0;
char * token1;
char fname[20],fipaddr[20],fport[20];
int user_found = 0;

	while(uname1[k] != NULL)
		{
            if(strcmp(uname1[k], uname) == 0) 
			{
				//  flag  = 1;
				//findex = k;
				
				printf("Search node Gotcha %s!\n",uname);
				strcpy(fname,uname1[k]); 
				strcpy(fipaddr,ipaddr1[k]);
				strcpy(fport,port1[k]);
				
				printf("found enteries are :%s,%s,%s\n",fname,fipaddr,fport);
				
				memset( sendbuff, '\0', sizeof( sendbuff ));
				
				strcpy(sendbuff,fname);
				strcat(sendbuff,"#");
				strcat(sendbuff,fipaddr);
				strcat(sendbuff,"#");
				strcat(sendbuff,fport);
				printf("%s\n",sendbuff);
				user_found = 1;
				break;
			}
		k++;
		}
	if(user_found != 1)
	{
		sendbuff = "N";
	}
	return sendbuff;
}

