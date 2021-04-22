#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/select.h>

int client_number = 0;
int sockfd, newsockfd;
char *socket_path = "\0hidden";
char tampon [100];
pid_t pid;

//Fermer puis spprimer la socket créee, et indiquer l'arrêt du programme
void quit(){
  printf("\n Signal intercepté ...\n");
  close(newsockfd);
  remove(socket_path);
  printf("Arret du serveur ...\n");
  exit(0);
}

//Lire le message reçu par un client, simuler le traitement de sa demande et lui renvoyer un message
int gererClient(struct sockaddr_un cli_addr, socklen_t clilen){

   bzero ((char *) tampon, 100);
   read (newsockfd, tampon, 100);
   printf ("Client n° %i a envoyé: %s\n", client_number, tampon);
   printf("Traitement des données ...\n");
   sleep(10); //simuler une action bloquante du serveur. Les autres clients ne devraient pas
              //être bloqués grâce au fork() et les processus lourds créés.
   printf("Fin du traitement pour le client %i\n",client_number);
   write (newsockfd, "Message reçu", 13);
   kill(getpid(),SIGTERM);

}

int main (int argc, char** argv){

 signal(SIGINT,quit);

 socklen_t clilen, servlen;
 struct sockaddr_un cli_addr;
 struct sockaddr_un serv_addr;

 fd_set active_fd_set, read_fd_set;

 if ( (sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
 {
   printf ("Erreur de creation de socket\n"); exit (1);
 }

 bzero((char *)&serv_addr, sizeof(serv_addr));
 serv_addr.sun_family = AF_LOCAL;
 strcpy(serv_addr.sun_path, "/tmp/socketLocale.01");
 servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

 if ( bind (sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
 {
   printf ("Erreur de bind\n"); exit (1);
 }

 listen(sockfd, 5);
 FD_ZERO(&active_fd_set);
 FD_SET(sockfd,&active_fd_set);

 int select_status;


 clilen = sizeof(cli_addr);
 read_fd_set = active_fd_set;

 while(1)
 {

   printf ("serveur: En attente sur /tmp/socketLocale.01...\n");
   newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr, &clilen);

   pid = fork();
   client_number ++;
   switch (pid)
      {
          case -1 : printf ("Erreur dans la creation du processus fils.\n");
          perror ("Erreur : ");
          break;
          case 0 : printf("Arrivée d'un nouveau client (client n° %i) \n",client_number);
          gererClient(cli_addr,clilen);
          break;
          default:
          break;

      }
    }

}
