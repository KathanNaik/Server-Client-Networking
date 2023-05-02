#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sockfd, newsockfd, clilen;
	char buffer[1024];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if(argc < 2)
		error("Provide Port No.\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error Opening socket\n");

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
		error("Error binding\n");

	while(1){
		listen(sockfd, 0);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

		struct sockaddr_in *cli = (struct sockaddr_in *)&cli_addr;
		unsigned char *ip = (unsigned char *)&cli->sin_addr.s_addr;
		printf("-----------------------------------------\n");
		printf("IP Address: %d.%d.%d.%d\nPort No.: %d\n", ip[0],ip[1],ip[2],ip[3],cli->sin_port); 
		printf("-----------------------------------------\n");

		if(newsockfd < 0)
			error("Error in accepting\n");

		bzero(buffer, 1024);
		n = read(newsockfd, buffer, 1023);
		if(n < 0)
			error("Error in reading\n");
		
		char req[7];
		for(int i = 0; i<6; i++)
			req[i] = buffer[i+5];

		printf("Current Request---\n\n%s\n", buffer);
		bool flag = true;
		FILE *fp;
				
		if(req[0] == '1' && req[1] == '.' && req[2] == 'h' && req[3] == 't' && req[4] == 'm' && req[5] == 'l')
			fp = fopen("1.html", "r");
		else if(req[0] == '2' && req[1] == '.' && req[2] == 'h' && req[3] == 't' && req[4] == 'm' && req[5] == 'l')
			fp = fopen("2.html", "r");
		else if(req[0] == '3' && req[1] == '.' && req[2] == 'h' && req[3] == 't' && req[4] == 'm' && req[5] == 'l')
			fp = fopen("3.html", "r");
		else { 
			fp = fopen("404.html", "r");
			flag = false;
		}

		char file[100];
		bzero(buffer, 1024);
		fread(file, 100, 1, fp);
		if(flag)
			strcat(buffer, "HTTP/1.1 200 OK\r\n");
		else
			strcat(buffer, "HTTP/1.1 404 Not Found \r\n");
		strcat(buffer, "Content-Length: 100\r\n\r\n");
		strcat(buffer, file);
		printf("Current Response---\n\n%s\n", buffer);

		n = write(newsockfd, buffer, 1023);
		if(n < 0)
			error("Error in writing\n");
	}

	return 0;
}
