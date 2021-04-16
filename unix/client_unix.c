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

  if (argc > 1) socket_path='\0' + argv[1];

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
/*
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
 }*/

   while(1)
  	{
  		printf("Saisir un message : ");
  		scanf("%s" , buf);

  		//Send some data
  		if( send(fd , buf , strlen(buf) , 0) < 0)
  		{
  			puts("Send failed");
  			exit(-1);
  		}

              //Receive a reply from the server
        if( recv(fd , server_reply , 100 , 0) < 0)
        {
          puts("recv failed");
          break;
        }

        puts("Serveur dit :");
        puts(server_reply);

  	}

  close(fd);
  printf("Fin\n");
  return 0;
}
