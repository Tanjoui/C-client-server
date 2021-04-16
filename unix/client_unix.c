#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//char *socket_path = "./socket";
char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  char buf[100], server_reply[100];
  int fd,rc;

  if (argc > 1){
    socket_path='\0' +argv[1];
  } else {
    printf("Veuillez spécifier un nom de socket valide en argument.\n");
    exit(1);
  }

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
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path));
  }

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

   while(1)
  	{
      bzero ((char *) buf, 100);
  		printf("Saisir un message : ");
  		scanf("%s" , buf);
      printf("Patientez pendant que le serveur traite votre message ... \n");

  		if( write(fd , buf , strlen(buf)) < 0)
  		{
  			puts("Send failed");
  			exit(-1);
  		}

      if( read(fd , server_reply , 100 ) < 0)
        {
          puts("recv failed");
          break;
      }

      printf("Serveur répond: %s \n", server_reply);

  	}

  close(fd);
  printf("Fin\n");
  return 0;
}
