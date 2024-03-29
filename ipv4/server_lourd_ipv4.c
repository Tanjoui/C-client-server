#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8888

int sockfd;
int newSocket;

//Fermer puis spprimer la socket créee, et indiquer l'arrêt du programme
void quit(){

  printf("\n Signal intercepté ...\n");
  close(newSocket);
  printf("Arret du serveur ...\n");
  exit(0);

}

int main(){

	int ret;
	 struct sockaddr_in serverAddr;

	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Erreur de connexion.\n");
		exit(1);
	}
	printf("[+]Server Socket crée.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Erreur de liaison au port 8888.\n");
		exit(1);
	}
	printf("[+]Liaison au port 8888 %d\n", 8888);

	if(listen(sockfd, 10) == 0){
		printf("[+]En recherche de Client....\n");
	}else{
		printf("[-]Erreur de liaison au Client.\n");
	}

	signal(SIGINT,quit);

	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connexion acceptée par %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Déconnecté de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}
