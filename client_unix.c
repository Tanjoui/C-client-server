#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stddef.h"
#include "unistd.h"

#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/un.h>
#include "sys/syscall.h"
#include <errno.h>

#include "af_unix_sockets_common.h"

/*
* AF_UNIX socket client, obtains an `AFUnixAddress` by opening the socket to the specified `path` and then invoking `#connect()` on the socket's file descriptor.
* Finally, the client reads input from `stdin` and writes that to the socket.
*/
void client(char *path) {
    fprintf(stdout, "Starting AF_UNIX client on Path=%s\n", path);
    AFUnixAddress *domainSocketAddress = open_af_unix_socket(path);
    printf("AF_UNIX client socket on Path=%s opened with fd=%d\n", domainSocketAddress->address->sun_path, domainSocketAddress->fd);

    int isConnected = connect(domainSocketAddress->fd, (struct sockaddr *)domainSocketAddress->address, sizeof(struct sockaddr));
    if(isConnected == -1) {
        fprintf(stderr, "Failed to connect to Path=%s. ErrorNo=%s\n", domainSocketAddress->address->sun_path, strerror(errno));
        cleanup(domainSocketAddress->fd, domainSocketAddress->address->sun_path);
        exit(errno);
    }

    char line[1024];
    while(fgets(line, 1024, stdin) != NULL) {
        int size = 0;
        for(;line[size] != '\0'; size++){
        }
        if(size == 0) {
            break;
        }
        int bytes = write(domainSocketAddress->fd, line, size);
    }

    cleanup(domainSocketAddress->fd, domainSocketAddress->address->sun_path);
    exit(0);
}