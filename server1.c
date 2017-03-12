
/* AUTHENTICATION SERVER */


#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#define LENGTH 1024

char * login(char buff[LENGTH]);
char * readline(FILE *fp);
int numoflines(FILE *fp);
int authenticateuser(FILE *fp, char user[20], char pwd[20], int n);
char * checkuser(FILE *fp, char user[1024], int n);
void signup(char buff[LENGTH]);
char current_user[20];



int main(int argc, char *argv[])
{
struct stat st = {0};
int ssockfd,clifd,portno,clilen,n,n1,n2;
char recvBuff[1024], tempa;
char sendBuff[1024];
struct sockaddr_in serv_addr,cli_addr;
char username[1204],password[1204],  *auth_login , *valid, tempBuff[1024];
FILE *fp;
FILE *fpcfg;
int pid;
int lines;
int num, i , j , temp[50],check,start;
int flag = 1;
char * token;

	/*create socket*/

	ssockfd = socket(AF_INET,SOCK_STREAM,0);
	
	if(ssockfd < 0)
	{
		printf("Error in opening socket");
		exit(1);
	}

	/*socket structure initializtion*/
	
	bzero((char *)&serv_addr,sizeof(serv_addr));
	portno = 6666;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	/*socket binding*/
	
	if (bind(ssockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		printf("\nBind Error");
		exit(1);
	}
	
	/*listen for clients*/
	listen(ssockfd,5);
	clilen = sizeof(cli_addr);
	
	/*Accept connecttion from client*/
	
	for(;;) {
		clifd = accept(ssockfd,(struct sockaddr *)&cli_addr,&clilen);
		if (clifd < 0){
			printf("\nError in accept");
			exit(1);
		}
		pid = fork();
		if (pid < 0) {
			printf("\nError in forking");
			exit(1);
		}
		if (pid == 0) {
			setbuf(stdout,NULL);
			printf("Success in forking\n");	
			close(ssockfd);
			while(1) {
				printf("About to read buffer\n");
				memset(recvBuff, '\0', sizeof(recvBuff));
				n = read(clifd, recvBuff, sizeof(recvBuff));
				if (n < 0) {
					printf("Error in read\n");
					exit(1);				
				}
				printf("Reading the value of buffer %s\n", recvBuff);
				printf("Here");
				if(recvBuff[strlen(recvBuff)-1] == '\n'){
						printf("Inside if");

					  recvBuff[strlen(recvBuff)-1]= '\0';
				}
				strcpy(tempBuff,recvBuff);
				token = strtok(tempBuff,"|");
				printf("after if");
				tempa = recvBuff[0];
				printf("\n value of temporary is %c", tempa);
				
				if(tempa == 'a')
				{
					printf("\n Selected Option is a");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff,"login");	
				}
				if(tempa=='b')
				{

					printf("\n Selected Option is b");
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff,"signup");	
				}
				// login 
				if(strcmp(token, "login")==0)
				{
					printf("\nverification of login");
					printf("\nrecvBuff = %s ", recvBuff);
					auth_login = login(recvBuff);

					if(strcmp(auth_login,"true")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "true");
						printf("\nin auth login true");
					}
					else if(strcmp(auth_login,"false")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "false");
						printf("\nin auth login false");
					}
				}
				//signup validate
				if(strcmp(token, "signup")==0)
				{
					printf("\nverification of signup");
					printf("\nrecvBuff = %s ", recvBuff);
					lines = numoflines(fp);
					valid = checkuser(fp,recvBuff,lines );
					printf("\n---> %s",valid);
					if(strcmp(valid,"exist")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "exist");
						printf("\nUser Exist");
					}
					else if(strcmp(valid,"dont")==0)
					{
						memset(sendBuff, '\0', sizeof(sendBuff));
						strcpy(sendBuff, "dont");
						printf("\nNew User");
					}
				}
				if(strcmp(token, "dosignup")==0)
				{
					printf("signing up");
					printf("\nrecvBuff = %s ", recvBuff);
					signup(recvBuff);
					
					memset(sendBuff, '\0', sizeof(sendBuff));
					strcpy(sendBuff, "login");
						printf("\nsend buff = %s",sendBuff);
				}
				n = write(clifd,sendBuff, strlen(sendBuff)+1);
				if(n < 0){
					printf("Write Error");
					exit(1);				
				}	
			}

			exit(0);	
		}
		else {
			close(clifd);		
		}	
	}
}


//FUNCTION TO COUNT THE NUMBER OF ENTRIES IN FILE

int numoflines(FILE *fp)
{

	int n=0,ch_count=0;
	fp = fopen("configuration.txt", "r");
	printf("Opening the file to count");
	while ((ch_count= fgetc(fp)) != EOF)
	{
		if (ch_count == '\n')
		n++;
	}
	fclose(fp);
	return n;

}

//check authentication 
int authenticateuser(FILE *fp, char user[20], char pwd[20], int n)
{
	char line_file[1024];
	int i,flag;
	int checkuser = 1;
	char * token;
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i <= n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			printf("line :%s" , line_file);

			if(line_file[strlen(line_file)-1 ]== '\n')
			{
			line_file[strlen(line_file)-1] = '\0';
			}
	
			token = strtok(line_file,"#");
			if(i<n)
			{
				if(checkuser==1)
				{
					if(strcmp(token,user)==0)
					{
				
						checkuser=0;
						flag=1;
					}
				}
			}
			if((i==n) && (checkuser==1))
			{
				
				flag=0;
				break;
			}
			if(flag==1)
			{
				token = strtok(NULL,"#");
				if(strcmp(token,pwd)==0)
				{
					flag=1;
					break;
				}
				else
				{
					
					flag=0;
					break;
				}
			
			}
		}
			
	}
	printf("\n flag= %d " ,flag);
	fclose(fp);
	return flag;	
}

//FUNCTION THAT CHECKS USER IS PRESENT IN THE FILE

char * checkuser(FILE *fp, char users[1024], int n)
{
	char * validate_user = malloc(20);
	char line_file[1024];
	int i,flag;
	char * token;
	char user[20];
	printf("IN A CHECK USER FIDGFD %s", users);
	token = strtok(users,"|");
	token = strtok(NULL,"|");
	strcpy(user,token);
	printf("\n-->%s user", user);
	fp = fopen("configuration.txt", "r");
	for(i=0 ; i < n ; i++)
	{
		if(fgets(line_file, 1024,(FILE *)fp)!=NULL)
		{
			//printf("line :%s" , line_file);
		}
		token = strtok(line_file,"#");
		if(strcmp(token,user)==0)
		{
			flag=1;
			break;
		}
		else
		{
			flag=0;
		}
	}

	fclose(fp);
	if(flag==1)
	{
		return "exist";
	}
	else
	{
		return "dont";
	}
	
}

//FUNCTION FOR SIGNUP OF USER IN THE FILE

void signup(char buff[LENGTH])
{
	char username[50], password[50], fname[50], lname[50], filenamesign[50];
	char *token;
	char signin[LENGTH];
	FILE *fpcfg;
	token = strtok(buff,"|");
	token = strtok(NULL,"|");
	strcpy(username, token);
	token = strtok(NULL,"|");
	strcpy(password, token);
	printf("In signup");
	fpcfg = fopen("configuration.txt", "a");
	fprintf(fpcfg,"%s#%s\n",username,password);
	fclose(fpcfg);
	
}

//FUNCTION FOR LOGIN OF USER

char * login(char buff[LENGTH])
{	
	printf("In login");
	char * token, username[20], password[20];
	char * auth_login = malloc(20);
	FILE * fp;
	int lines, check;
	memset(username,0,20);
	memset(password,0,20);
	memset(current_user,0,20);
	token = strtok(buff,"|");
    printf("\ntoken = %s",token);
	token = strtok(NULL, "|");
	strcpy(username, token);
	printf("\nusername = %s absfdf",username);
	token = strtok(NULL, "|");
	strcpy(password,token);
	printf("\npassword = %s dgfdhjhhf",password);
	lines = numoflines(fp);
	strcpy(current_user,username);
	check = authenticateuser(fp, username, password,lines);
	if(check == 1 )
	{
		return "true";
	}
	if(check == 0)
	{
		return "false";
	}
}
