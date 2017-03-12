
/* CLIENT FILE */

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#define LENGTH 1024

void start_chat(int sock_fd, struct sockaddr_in *peer, unsigned long r);
int *chat();
int communicate();
char * signup_valid(); 
void clean_stdin(void);						
char * login();
char * signup(); 
char * concat();	   
char * iplookup();
char signupuser[50];
char current_user[20];
int portno1;
char *ip;
char choice2[2];
char *user[LENGTH];
char ipad[50];
char prt[50];
int flag = 0;
int flag1;
char buffer1[LENGTH];
char r_user[20];
int temp;
int n;
int findex;
char *c_i;
char *c_p;
int CHAT = 0;
int i  = 0;
char tempip[20][20];
char tempuser[20][20];
struct sockaddr_in serv_addr;
char sip[20] = "";
char sport[10] = "";

//MAIN PROGRAM
int main(int argc, char *argv[])
{
  int ssockfd,portno,n1,ch;
  int newsockfd;
  char choice[2];
  
  char username[1024],password[1024];
  
  struct hostent *server;
  struct hostent *supernodeserver;
  char buffer[1024];
  char superbuffer[1024];
  char sendBuff[LENGTH], recvBuff[LENGTH],tempBuff[LENGTH];
  char * loginDetail, *token_set, *token,  *signupvalid, *signupdetail; 
  char *supernodeip;
  int auth;
  
  char * extra;
  
  
//ERROR: IP OR PORT
  if (argc < 3){
    printf("Usage hostname portno");
    exit(1);
  }
  
  portno = atoi(argv[2]);
  ssockfd = socket(AF_INET, SOCK_STREAM,0);
  
  if(ssockfd < 0)
    {
      printf("Error in opening socket");
      exit(1);
    }
  
  server = gethostbyname(argv[1]);
  
  printf("\n---- argv[1] is :%s\n",argv[1]);
 
  //NO SERVER HOST EXIST ERROR
  if(server == NULL){
    printf("No such host exist");
    exit(1);
  }

  bzero((char *)&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(portno);
    
  //SOCKET CONNECTION
  if(connect(ssockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
      printf("ERROR connecting");
      exit(1);
    }
    
  printf("\nConnected to server");
  setbuf(stdout,NULL);
    
  while(1) {
    printf("Select a for Login: \n");
    printf("Select b for Registration: \n");
    fgets(choice,sizeof(choice),stdin);
    memset(buffer,'\0', sizeof(buffer));
    buffer[0] = choice[0];
    printf("\n Choice selected by user is: %c", buffer[0]);
    n = write(ssockfd, buffer, strlen(buffer) + 1);
    printf("\n I am writing to server");
    printf("\nAfter writing the option to server");
      
    while(1){
      memset(recvBuff,'\0',sizeof(recvBuff));
      n = read(ssockfd,recvBuff,sizeof(recvBuff));
      printf("\n---recv buff ---- %s\n", recvBuff);
        
      if(strcmp(recvBuff,"signup") == 0)
      {
	  //SIGNUP SERVICE
	  printf(" signup");
	  memset(sendBuff , '\0', sizeof(sendBuff));
	  signupvalid = signup_valid();
	  strcpy(sendBuff,"signup");
	  strcat(sendBuff,"|");
	  strcat(sendBuff,signupvalid);
	  printf("-->%s\n",sendBuff );		
	  n= write(ssockfd, sendBuff, strlen(sendBuff)+1 );
	  
      }
        
      if(strcmp(recvBuff,"exist") == 0)
      {
	  printf("\nThis username is already exist please choose different username\n");
	  memset(recvBuff,'\0',sizeof(recvBuff));
	  break;
      }
        
      if(strcmp(recvBuff,"dont") == 0)
      {
	  printf("\nsuccess\n");
          
      //SIGNUP FUNCTION CALL
	  signupdetail = signup();
	  memset(sendBuff , '\0', sizeof(sendBuff));
	  strcpy(sendBuff,"dosignup");
	  strcat(sendBuff,"|");
	  strcat(sendBuff,signupdetail);
	  printf("-->%s\n",sendBuff );
	  
	  
	  n= write(ssockfd, sendBuff, strlen(sendBuff)+1 );
	  
	  
      }
        
      // LOGIN SERVICE
      if(strcmp(recvBuff,"login")==0)
      {
      //LOGIN FUNCTION CALL
	  loginDetail=login();
	  
	  //WRITING LOGIN DETAILS TO SERVER
	  memset(sendBuff,'\0',sizeof(sendBuff));
	  strcpy(sendBuff,"login");
	  strcat(sendBuff, "|");
	  strcat(sendBuff,loginDetail);
        
	  n= write(ssockfd, sendBuff, strlen(sendBuff)+1 );
	  
      }
        
      //AFTER AUTHENCTICATION IS DONE
      
      
      if(strcmp(recvBuff,"true") == 0)
      {
	  printf("\nin rcv true");
	  token = strtok(loginDetail, "|");
	  strcpy(current_user, token);
	  system("clear");
	  printf("\n\t\t\t WELCOME %s \t\t\n", current_user);
	  auth = 1;
	  break;
	  }
        
      //LOGIN FAIL
        
      if(strcmp(recvBuff,"false") == 0)
      {
	  system("clear");
	  printf("\n USERNAME AND PASSWORD DO NOT MATCH PLEASE TRY AGAIN\n");
	  memset(recvBuff,'\0',sizeof(recvBuff));
	  break;
      }
      
      
    }//1st while loop closed
    
    close(ssockfd);
    
    //SUPERNODE REGISTRATION
    if(auth == 1)
      
    {
	
	printf("\nEnter the supernode IP:\n");
	fflush(stdin);
	fflush(stdout);
	char * supernodeip = malloc(500);
	scanf("%s",supernodeip);
	printf("\n The supernode IP is: %s",supernodeip);
	printf("\nEnter the supernode's port number:\n");
	scanf("%d",&portno1);
	newsockfd = socket(AF_INET, SOCK_STREAM,0);
	printf("\nsockect created\n");
        
	if(newsockfd < 0)
	  {
	    perror("\nError in opening new socket\n");
	    exit(1);
	  }
	
	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(supernodeip);
	serv_addr.sin_port = htons(portno1);
	printf("\nbefore connected ");
	sprintf(sip,"%s",supernodeip);
	sprintf(sport,"%d",portno1);
	printf("\n sip = %s",sip);
	
	if(connect(newsockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	  {
	    printf("\nERROR connecting");
	    exit(1);
	  }
	printf("\nConnected to supernode server");
	printf("\nPlease enter the message:\n ");
	fflush(stdin);
	fflush(stdout);
	extra = concat();
	
	strcat(superbuffer,extra);
	
	/* SEND MESSAGE TO SERVER */
	n = write(newsockfd, superbuffer, strlen(superbuffer));
	
	if (n < 0) {
	  printf("\nERROR writing to socket");
	  exit(1);
	}
	
	/* READ SERVER RESPONSE */
	
	fflush(stdin);
	n = read(newsockfd, superbuffer, 1024);
	
	if (n < 0)
    {
	  printf("ERROR reading from socket");
	  exit(1);
	}
	
	printf("%s\n",superbuffer);
	close(newsockfd);
        
	while(1)
	  {	
	    flag1 = 1;
          
        //COMMUNICATE FUNCTION CALL
	    if(communicate())
	  	{
		    chat();
		}
	    else
		{
		    printf("\n Closing client\n");
		    break;
		}
	  }
    }
    
    printf("After auth == 1");
    break;
  }//while closed
}


//COMMUNICATE FUNCTION WHICH ASKS USER TO COMMUNICATE WITH THEIR FRIENDS OR NEW PERSON AND CONNECTS ACCORDINGLY TO USER OR VIA SUPERNODE

int communicate()
{
  char *token1;
  if(flag1 == 1)
    {
      printf("---------Communicate with:(a1)Known User or (b1) New User or (e)Exit--------\n");
      fflush(stdin);
      fflush(stdout);
      scanf("\n%s",choice2);
        
      if(strcmp(choice2,"e") == 0)
	  {
	  return 0; 
	  }
      memset(buffer1,'\0', sizeof(buffer1));
      printf("\nEnter Username\n");
      scanf("%s", r_user);
     
      if(strcmp(choice2,"a1") == 0)
      {
	  printf("\na1 selected\n");
	  temp = search();
	  if(temp == 1)
	    {
	      printf("\nUser exist\n");
		return 1;
	    }
	  else
	    {
	      printf("\nUser not exist\n");
	    }
	
      }
        
      if(strcmp(choice2,"b1") == 0)
      {
	  //printf("hhiiii");
	  int nsockfd;
	  char * supernodeip1 = malloc(20);
	  
	  nsockfd = socket(AF_INET, SOCK_STREAM,0);
	  printf("\nsocket created\n");
        
	  if(nsockfd < 0)
	    {
	      perror("\nError in opening new socket\n");
	      exit(1);
	    }
	  
	  
	  bzero((char *)&serv_addr,sizeof(serv_addr));
	  serv_addr.sin_family = AF_INET;
	  serv_addr.sin_addr.s_addr = inet_addr(sip);
	  serv_addr.sin_port = htons(atoi(sport));	
	  
	  if(connect(nsockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	    {
	      printf("\nERROR connecting");
	      exit(1);
	    }
          
	  strcpy(buffer1,"conn");
	  strcat(buffer1,"#");		
	  strcat(buffer1,r_user);
	  n = write(nsockfd, buffer1, sizeof(buffer1));
	  
	  if (n < 0)
      {
	    printf("\nERROR writing to socket");
	    exit(1);
	  }
          
	  n = read(nsockfd, buffer1, sizeof(buffer1));
	  printf("the Rec is : %s",buffer1);
	  token1 = strtok(buffer1,"#");
	  strcpy(user,token1);
      strcpy(tempuser[i],user);
	  printf("\n user received in array = %s\n", user);
	  token1 = strtok(NULL,"#");
	  c_i = token1;
	  strcpy(ipad,token1);
      strcpy(tempip[i],ipad);
	  printf("\n ip received in array = %s\n", ipad);
	  token1 = strtok(NULL,"#");
	  c_p = token1;
	  strcpy(prt,token1);
	  printf("\n port received in array = %s\n", prt);
	  close(nsockfd);
	  return 1;
	  
	}
  }
}

//SEARCH FUNCTION FOR SUPERNODE TO SEARCH FOR THE USERNAME AND RETURN IP AND PORT

int  search()
{

//char tempuser;
char enteruser;
int k = 0;
int j;
printf("\n in search");

int user_found = 0;
	
	while(k<20)
		{
		printf("\n in while");
		
        if(strcmp(tempuser[k],r_user) == 0)
        {
            printf("Search node Gotcha %s!\n",r_user);
            strcpy(c_i,tempip[k]);
            printf("found ip is :%s\n",c_i);
            flag = 1;
            printf("\n flag value = %d", flag);
            break;
			}
		k++;
		}
        if(flag != 1)
        {
            flag = 0;
        }
        return flag;
        }


// FUNCTION TO REMOVE THE STDIN BUFFER BY REMOVING \n

void clean_stdin(void)
{
  int c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);
}

//FUNCTION TO RETURN USER'S INPUT FOR SIGNUP WITH VALIDATION

char * signup_valid()
{
  char * signupDetail = malloc(20);
  printf("\nEnter Username: ");
  clean_stdin();
  fgets(signupuser,sizeof(signupuser), stdin);
  
  if(signupuser[strlen(signupuser)-1 ]== '\n')
    {
      signupuser[strlen(signupuser)-1] = '\0';
    }
  strcpy(signupDetail, signupuser);
  return signupDetail;

}

//FUNCTION FOR SIGNUP DETAILS

char * signup()
{
  char * sign = malloc(500);
  char tempo[50];
  strcpy(sign, signupuser);
  strcat(sign,"|");
  printf("\nEnter a password : ");
  fgets(tempo,sizeof(tempo), stdin);
  
  if(tempo[strlen(tempo)-1 ]== '\n')
    {
      tempo[strlen(tempo)-1] = '\0';
    }
  strcat(sign, tempo);
  strcat(sign,"|");
  return sign;
  
}

//FUNCTION FOR LOGIN DETAILS AND RETURNS THE INFO

char * login()
{
  char * loginDetail = malloc(20);
  char password[20], username[20];
  
  printf("\nEnter Username: ");
  
  fflush(stdin);
  scanf("%s", current_user);
  
  if(current_user[strlen(current_user)-1 ]== '\n')
    {
      current_user[strlen(current_user)-1] = '\0';
    }
  
  strcpy(username,current_user);
  fflush(stdin);
  printf("\nEnter Password: ");
  scanf("%s", password);
 
  if(password[strlen(password)-1 ]== '\n')
    {
      password[strlen(password)-1] = '\0';
    }
    
  strcat(username,"|");
  printf("%s",username);
  strcat(username,password);
  strcpy(loginDetail, username);
  printf("\nlogin detail = %s hii", loginDetail);
  return loginDetail;
}

//FUNCTION TO DIFFERENTIATE WHETHER THE INFORMATION IS FOR REGISTRATION AND CONCATE THE STRING WITH '#'

char *concat()
{
  printf("1");
  iplookup();
  
  printf("2");
  char *extra = malloc(500);
  char *port_s = malloc(500);
  printf("3");
  strcpy(extra,"reg");
  strcat(extra,"#");
  strcat(extra,current_user);
  printf("4");
  strcat(extra,"#");
  printf("5");
  strcat(extra,ip);
  printf("5");
  strcat(extra,"#");
  
  sprintf(port_s, "%d", portno1);
  strcat(extra,port_s);
  printf("extra is %s",extra);
  return extra;
}

//FUNCTION RETURNS IP
char *iplookup()
{
  int fd;
  struct ifreq ifr;
  
  
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  
 /* I want to get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;
  
  /* I want IP address attached to "eth0" */
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
  
  ioctl(fd, SIOCGIFADDR, &ifr);

  close(fd);
  
  /* display result */
  printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
  ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  printf("%s\n", ip);
  return ip;
}

//FUNCTION TO START CHATTING BETWEEN USER AFTER GETTING IP AND PORT

void start_chat(int sock_fd, struct sockaddr_in *peer, unsigned long remote_p)
{
  
  int ret;
  ssize_t bytes;
  char input_buffer[1024];
  char output_buffer[1024];
  struct pollfd fds[2];
  struct sockaddr_in r_addr;
  
  struct sockaddr_in _addr;
  
  /* Descriptor zero is stdin */
  fds[0].fd = 0;
  fds[1].fd = sock_fd;
  fds[0].events = POLLIN | POLLPRI;
  fds[1].events = POLLIN | POLLPRI;
  
  /* Normally we'd check an exit condition, but for this example
   * we loop endlessly.
   */
  while (1) {
    /* Call poll() */
    ret = poll(fds, 2, -1);
    char exittemp[5];
    sprintf(exittemp,"%s","EXIT");
    
    if (ret < 0) {
      printf("Error - poll returned error: %s\n", strerror(errno));
      break;
    }
    if (ret > 0) {
      
      /* Regardless of requested events, poll() can always return these */
      if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        printf("Error - poll indicated stdin error\n");
        break;
      }
      if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        printf("Error - poll indicated socket error\n");
        break;
      }
      
      /* Check if data to read from stdin */
      if (fds[0].revents & (POLLIN | POLLPRI)) {
        bytes = read(0, output_buffer, sizeof(output_buffer));
        if (bytes < 0) {
          printf("Error - stdin error: %s\n", strerror(errno));
          break;
        }
	
	bytes = sendto(sock_fd, output_buffer, bytes, 0,
                       (struct sockaddr *)peer, sizeof(struct sockaddr_in));
	
	if (bytes < 0) {
          	printf("Error - sendto error: %s\n", strerror(errno));
          	break;
	}
	
	
	
	printf("Sending: %.*s\n", (int)bytes, output_buffer);
	if(strncmp(exittemp,output_buffer,4) == 0)
	  {	
	    printf("\nin exit");
	    flag1 = 1;
	    return;
	  }
	
       	
      }
      int len;
      len = sizeof(r_addr);
      
      /* Check if data to read from socket */
      if (fds[1].revents & (POLLIN | POLLPRI)) {
	bytes = recvfrom(sock_fd, input_buffer, sizeof(input_buffer),
                         0, &r_addr, &len);
          
	if(strcmp(input_buffer, "BUSY") == 0)
        {
	    printf("Received: %.*s\n", (int)bytes, input_buffer);
	    return;
        }
	
	
	if(strncmp(exittemp,input_buffer,4) == 0)
        {
	    flag1 = 1;
	    return;
        }
	
    if (bytes < 0)
        {
          printf("Error - recvfrom error: %s\n", strerror(errno));
          break;
        }
          
    if (bytes > 0)
        {
	  int r_p = ntohs(r_addr.sin_port);
	  printf("reciving port : %d\n", r_p);
            
        if(r_p != remote_p)
	    {
	      struct sockaddr_in my_addr;
	      my_addr.sin_family = AF_INET;
	      my_addr.sin_port = htons(r_p);
	      my_addr.sin_addr = r_addr.sin_addr;
	      struct sockaddr_in *my_peer = &my_addr;
          bytes = sendto(sock_fd, "BUSY", bytes, 0,
			     (struct sockaddr *)my_peer, sizeof(struct sockaddr_in));
	      
        if (bytes < 0) {
          	printf("Error - sendto error: %s\n", strerror(errno));
          	break;
	      }		
	    }else{
	    printf("Received: %.*s\n", (int)bytes, input_buffer);		
	  }
        }

	
	
      }
    }
  }
}

//FUNCTION ASKING FOR EACH OTHER'S PORT AND START CHATTING

int *chat()
{
  printf("entered chat");
  unsigned long local_port;
  unsigned long remote_port;
  int sock_fd;
  
  struct sockaddr_in server_addr;
  struct sockaddr_in peer_addr;
  char * remoteaddr = malloc(500);
  

  printf("\n enter local port\n");
  scanf("%lu",&local_port);
    
  if (local_port < 1 || local_port > 65535) {
    printf("Error - invalid local port '%lu'\n", local_port);
    exit(1);
  } 
  printf("\nenter remote_port\n");
  fflush(stdout);
  fflush(stdin);
  scanf("%lu",&remote_port);
    
  if (remote_port < 1 || remote_port > 65535) {
    printf("Error - invalid remote port '%lu'\n",remote_port);
    exit(1);
  }
  
  printf("\nenter remote ip address\n");
  printf("\n Remote ip is :%s\n",c_i);
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons(remote_port);
    
  if (inet_aton(c_i, &peer_addr.sin_addr) == 0) {
    printf("Error - invalid remote address '%s'\n", c_i);
    exit(1);
  }
  
  /* Create UDP socket */
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
  if (sock_fd < 0) {
    printf("Error - failed to open socket: %s\n", strerror(errno));
    exit(1);
  }
  
  /* Bind socket */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(local_port);
    
  if (bind(sock_fd, (struct sockaddr *)(&server_addr),
           sizeof(server_addr)) < 0) {
    printf("Error - failed to bind socket: %s\n", strerror(errno));
    exit(1);
  }
  
  /* Call chat handler to loop */
  start_chat(sock_fd, &peer_addr, remote_port);
  
  close(sock_fd);
  
  return 0;
}
