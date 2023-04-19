 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <pthread.h>
 
 void error (const char *msg){
 	perror(msg);
 	exit(1);
 
 }
 
 void client_handle(void *temp) {
 
 	int newsockfd = *((int *) temp);
 
 	int n;
	
	char buffer [255];
 	
  while(1){
  	bzero(buffer, 255);
  	n = read(newsockfd, buffer , 255);
  	if(n<0)
  	error("error on reading");
  	printf(buffer);
  	bzero (buffer, 255);
  	fgets (buffer, 255, stdin);
  	
  	n = write(newsockfd, buffer , strlen(buffer));
  	
  	if(n<0)
  	error("error on writing");
  	
  	int i = strncmp("Bye", buffer, 3);
  	if (i == 0)
  	break;
  
  }	
  
  close(newsockfd);
 
 }
 
 int main(int argc, char * argv[]){
 	if (argc<2){
 		printf("error in passing arguments");
 		exit(1);
 	}
 	
  pthread_t th[10];
  int thread_count;
 	
  int sockfd, portno; //n, newsockfd,
  //char buffer [255];
 
  struct sockaddr_in serv_addr; //cli_addr,
  //socklen_t clilen;
 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
  if(sockfd<0){
 	printf("error opening socket");
  }
 
  bzero((char *) &serv_addr, sizeof (serv_addr));
  portno = atoi(argv[1]);
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  
  if(bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) )<0)
  error("binding failed");
  
  listen(sockfd ,5);
  
  while(1){
  
  int newsockfd;
	
  socklen_t clilen;
  struct sockaddr_in cli_addr;
 	
  clilen = sizeof (cli_addr);
  
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  
  if(newsockfd<0){
  	printf("error in accept");
  }
  
  int *arg = malloc(sizeof(*arg));
  
  *arg = newsockfd;
  
  pthread_create(&th[thread_count], NULL , &client_handle, arg);
  thread_count++;
  
  }
  
 	close(sockfd);
 	return 0;
 }
