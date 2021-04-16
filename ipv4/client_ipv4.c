#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc, char **argv)
{
    //initialization des buffer et du socket
    int sockfd;
    char buffer[1000];
    char server_reply[2000];
    ssize_t n;
  
   struct sockaddr_in servaddr; 
   
   //creation du socket ipv4
   sockfd = socket(AF_INET,SOCK_STREAM,0);
   if (sockfd == -1)
    {
        perror("Could not create socket");
    }
	printf("Created Socket \n");


   bzero(&servaddr,sizeof (servaddr)); //mise à zero du socket
   //parametrage du socket (port/ip/type d'addresse)
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(8888);
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
   //connexion au socket
   connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));
   //boucle d'écriture des messages
   while (1)
   {
    printf("Entrez un message: ");
    scanf("%s",buffer);

    if (send(sockfd,buffer,strlen(buffer),0) < 0)
    {
	   printf("Erreur \n");
	   return 1;
    }
    if(recv(sockfd,server_reply,2000,0 ) < 0)
    {
	   puts("Erreur");
	   break;
    }
    // remise à zero des buffer pour les prochains message
   printf("Réponse du serveur: %s \n",server_reply );
   bzero(&server_reply, sizeof(server_reply));
   memset(&server_reply,0,sizeof(server_reply));
   }
    //fermeture du socket
    close(sockfd);
	return 0;
}