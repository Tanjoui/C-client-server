#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
<<<<<<< HEAD

//char *socket_path = "./socket";
char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  char buf[100];
  int fd,rc;

  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_LOCAL;
  if (*socket_path == '\0') {
    *addr.sun_path = '\0';
    strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
  }

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  while(1) {
      printf("Taper le message à envoyer ... \n");
      if((rc=read(STDIN_FILENO, buf, sizeof(buf))) > 0){
        if (write(fd, buf, rc) != rc) {
          if (rc > 0) fprintf(stderr,"partial write");
          else {
            printf("Fin1\n");
            perror("write error");
            exit(-1);
          }
        }
    }else{
      printf("Fin2\n");
    }
 }

  printf("Fin\n");
  return 0;
}
=======
int main(int argc, char **argv)
{
  int
      sockfd,
      servlen;
  struct sockaddr_un serv_addr;
  char tampon[30];
  int nbOctets;
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, "/tmp/socketLocale.1");
  servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
  {
    printf("Erreur de creation de socket\n");
    exit(1);
  }
  if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
  {
    printf("Erreur de creation de connect\n");
    exit(1);
  }
  write(sockfd, "Bonjour le serveur !", 20);
  printf("Client envoie : Bonjour le serveur !\n");
  nbOctets = 0;
  bzero((char *)tampon, 30);
  read(sockfd, tampon,
       13);
  printf("Client reçoit : %s\n", tampon);
}
close(sockfd);
>>>>>>> a0ef9a7faf71b372dae45f8c77d5ab73b50659c3
