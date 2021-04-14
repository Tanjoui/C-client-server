/*
Client ipv4 
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Créér un socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Le socket n'a pas pu être créé");
	}
	puts("Socket créé");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //on choisit le port 8888 en localhost
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connexion au serveur
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Connexion échouéé");
		return 1;
	}
	
	puts("Connecté\n");
	while(1)
	{
		printf("Entrez un message : ");
		scanf("%s" , message);
		
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Envoi échoué");
			return 1;
		}
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("Récéption échouée");
			break;
		}
		
		puts("Réponse du serveur :");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}