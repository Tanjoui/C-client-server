

//server processus lourds socket Unix
//server processus leger socket Unix
//server process lourds ipv4
//server process leger ipv4

//client unix
//client ipv4



/*
	C socket server example, handles multiple clients using threads
*/

#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//1 creation de la structure sockaddr
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );

	//2 appel de la fonction socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Le socket n'a pas pu être créé");
	}
	puts("Socket créé");
	
	//3 Appel de la fonction bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("Bind échoué");
		return 1;
	}
	puts("Bind effectué");
	
	//4 appel de la fonction listen
	listen(socket_desc , 3);
	
	puts("Attente des connexions entrantes...");
	c = sizeof(struct sockaddr_in);
	
	//5 boucle d'attente de la fonction accept
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		//5.2 lorsqu'on reçoit une demande, génère un socket file
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("Le thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("Echec de la fonction accept");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Genre un socket file
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	
	//Utiliser le descripteur pour dialoguer avec le sclient
	message = "Connexion établie avec le serveur\n";
	write(sock , message , strlen(message));
	
	message = "Entrez un message à envoyer au serveur: \n";
	write(sock , message , strlen(message));
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
	}
	
	if(read_size == 0)
	{
		puts("Client déconnécté");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//6 fin du dialogue, on ferme la socket file
	free(socket_desc);
	
	return 0;
}