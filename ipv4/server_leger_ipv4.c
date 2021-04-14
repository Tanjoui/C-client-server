#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //ATTENTION, ajouter tag -lpthread lors de la compilation


void *server_handler (void *fd_pointer);

int main()
{
	//Creation structure sockaddr
    int listenfd, connfd, *new_sock;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr; 

	//2 Appel fonction socket
   listenfd = socket(AF_INET,SOCK_STREAM,0);
   if (listenfd == -1)
   {
	  perror("La socket n'a pas pu être créée \n"); 
   }
	puts("Socket Créée");
  
  
   bzero(&servaddr,sizeof (servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = INADDR_ANY;
   servaddr.sin_port = htons(8888);
   
   //3 Appel fonction bind
   if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
   {
	   perror("bind échoué");
	   return 1;
   }
   puts("bind réussi");
   //4 Appel fonction listen
   listen(listenfd, 5);
   

   puts("En attente de connxions entrantes au port 8888");
   clilen = sizeof(cliaddr);
    while ((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen)))
  
	{
		puts("Connexion accéptée");
		
		pthread_t server_thread;
        new_sock = malloc(1);
        *new_sock = connfd;
		//5.2 on créé un processus leger
		pthread_create(&server_thread,NULL,server_handler,(void*) new_sock);
	}
	if (connfd < 0)
	{
		perror("Accecpt échoué");
		return 1;
	}
    //nouvelle itération de boucle
	return 0;
  
   //close(connfd);
}

/**
 * 5.3 Dialogue entre le rpocessus fils et le client grace à la socket fille
 **/
void *server_handler (void *fd_pointer)
{
	printf("Dialogue client/serveur \n");
	int sock = *(int *)fd_pointer;
    int read_size, write_size;
    char *message;
	static char client_message[2000];
    message = " \nDialogue client/serveur \n";


	static int send_once = 0;
	if (send_once < 1)
	{
	send_once++;
	}

    while((read_size = recv(sock,client_message,2000,0)) > 0)
   {
     printf("Le message reçu a pour taille %d \n", read_size);
     write(sock,client_message,strlen(client_message));
   }
    if(read_size == 0)
    {
        puts("Client déconnécté");
        fflush(stdout);
    }
	else if(read_size == -1)
    {
        perror("Récéption échouée");
    }
    free(fd_pointer);
     
    return 0;
}