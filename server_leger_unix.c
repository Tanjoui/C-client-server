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
* Open a `AF_UNIX` socket on the `path` specified. `bind()` to that address, `#listen()` for incoming connections and `accept()`. Finally, wait for input from the socket and print 
* that to the `stdout`. When one connection is closed, wait for the next one.
*/
void server(char *path) {
    printf("Starting AF_UNIX server on Path=%s\n", path);
    AFUnixAddress *domainSocketAddress = open_af_unix_socket(path);

    int hasBind = bind(domainSocketAddress->fd, (struct sockaddr *)domainSocketAddress->address, sizeof(struct sockaddr));
    if(hasBind == -1){
        fprintf(stderr, "Failed to bind AF_UNIX socket on Path=%s. ErrorNo=%d\n", path, errno);
        cleanup(domainSocketAddress->fd, path);
        exit(errno);
    }

    int isListening = listen(domainSocketAddress->fd,  10);
    if(isListening == -1) {
        fprintf(stderr, "Failed to listen to AF_UNIX socket on Path=%s. ErrorNo=%d\n", path, errno);
        cleanup(domainSocketAddress->fd, path);
        exit(errno);
    }

    fprintf(stdout, "Start accepting connections on Path=%s\n", path);
    while(TRUE) {
        int connFd = accept(domainSocketAddress->fd, NULL, NULL);
        if(connFd == -1) {
            fprintf(stderr, "Error while accepting connection. Error=%s, ErrorNo=%d\n", strerror(errno), errno);
            cleanup(domainSocketAddress->fd, path);
            exit(errno);
        }

        char buf[BUFSIZ];
        while(TRUE){
            int bytes = read(connFd, buf, BUFSIZ);
            if(bytes <= 0) {
                fprintf(stdout, "Connection closed\n");
                break;
            }
            write(1, buf, bytes);
        }
    }

    cleanup(domainSocketAddress->fd, path);
}