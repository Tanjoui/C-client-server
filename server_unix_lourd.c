#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc , char *argv[])
{

  int sockfd, newsockfd;
  socklen_t client_len, server_len;
  char buff[32];
  int octetslen;

  struct sockaddr_un client_addr;
  struct sockaddr_un server_addr;

	if(sockfd = socket(AF_LOCAL,SOCK_STREAM,0)==-1){
    printf("Erreur: creation socket\n", );
    exit(1);
  }

//  bzero((char *) &server_addr, sizeof(server_addr));
  memset(&server_addr,0,sizeof(addr));

  server_addr.sun_family=AF_LOCAL;

  strcpy(server_addr.sun_path,"/tpm/socket");
  server_len = strlen(server_addr.sun_path) + sizeof(server_addr.sun_family);

  if(bind(sockfd, (struct sockaddr *) &server_addr, server_len)==-1){
    printf("Erreur: binding socket\n", );
    exit(1);
  }

  listen(sockfd,3);

  while(true){
    client_len = sizeof(client_addr);
    printf("Attente de clients...");
    newsockfd = accept (sockfd,(struct sockaddr*) & client_addr, client_len);

    if(newsockfd==-1){
      printf("Erreur: connexion refusée\n", );
    }

    nbOctets = 0;
    memset(&buff,0,sizeof(buff));

    read(newsockfd,buff,32);
    printf("Message reçu: %s\n", buff );

    write(newsockfd,"Ok",2);

    close(newsockfd);

  }

}
