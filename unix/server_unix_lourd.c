#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/select.h>

static int running = 1;
int sockfd, newsockfd;
char *socket_path = "\0hidden";

void quit(){
  printf("\n Signal intercepté ...\n");
  close(newsockfd);
  running = 0;
}

int main (int argc, char** argv)
{

 if (argc > 1) socket_path=argv[1];

 signal(SIGINT,quit);

 socklen_t clilen, servlen;
 struct sockaddr_un cli_addr;
 struct sockaddr_un serv_addr;
 char tampon [30];
 int nbOctets;

 struct timeval timeout;
 timeout.tv_sec = 3;
 timeout.tv_usec = 0;

 fd_set active_fd_set, read_fd_set;

 if ( (sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
 {
   printf ("Erreur de creation de socket\n"); exit (1);
 }

 bzero((char *) &serv_addr, sizeof(serv_addr));
 serv_addr.sun_family = AF_LOCAL;
// strcpy(serv_addr.sun_path, "\0/tmp/unixLocalSocket.1");
  if (*socket_path == '\0') {
    *serv_addr.sun_path = '\0';
    strncpy(serv_addr.sun_path+1, socket_path+1, sizeof(serv_addr.sun_path)-2);
  } else {
    strncpy(serv_addr.sun_path, socket_path, sizeof(serv_addr.sun_path)-1);
  }

 servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

 if ( bind (sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
 {
   printf ("Erreur de bind\n"); exit (1);
 }

 listen(sockfd, 5);
 FD_ZERO(&active_fd_set);
 FD_SET(sockfd,&active_fd_set);

 int select_status;

 while (running==1)
 {

   clilen = sizeof(cli_addr);
   printf ("serveur: En attente...\n");

   read_fd_set = active_fd_set;
   select_status = select(FD_SETSIZE,&read_fd_set, NULL,NULL,NULL);

   if(select_status>0 && running==1){
     newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0)
      printf ("serveur: Erreur de accept\n");

     nbOctets = 0;
     bzero ((char *) tampon, 30);

     read (newsockfd, tampon, 20);
     printf ("Serveur reçoit : %s\n", tampon);

   }

  // write (newsockfd, "Message reçu", 13);
   //printf ("Serveur envoie : Message recu\n");

 }

 printf("Arret du serveur ...\n");
 exit(0);

}
