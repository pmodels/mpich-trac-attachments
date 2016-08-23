#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>

///   SERVER   

#define LEN (2147483648)

int main(int argc, char *argv[])
{
    //Declaring process variables.
    int server_sockfd, client_sockfd;
    int server_len ; 
    int rc ; 
    unsigned client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    struct sockaddr_in sock_id;
    socklen_t len;
    ssize_t ret;
    char *array;
    struct iovec iov[2];
    char header[40] = {0};
    struct pollfd fds[1];

    array = malloc(LEN);
    assert(array);
    memset(array, 1, LEN);

    //Remove any old socket and create an unnamed socket for the server.
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(0) ; 
    server_len = sizeof(server_address);

    rc = bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
    assert(rc == 0);

    len = sizeof(sock_id);
    rc = getsockname(server_sockfd, (struct sockaddr *)&sock_id, &len);
    assert(rc == 0);

    printf("port = %hu\n", ntohs(sock_id.sin_port));
        
        
    //Create a connection queue and wait for clients
    rc = listen(server_sockfd, 5);
    assert(rc == 0);

    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
    printf("after accept()... client_sockfd = %d\n", client_sockfd) ;

            
    iov[0].iov_base = header;
    iov[0].iov_len = 40;
    iov[1].iov_base = array;
    iov[1].iov_len = LEN;

    fds->fd = client_sockfd;
    fds->events = POLLIN;

    /* wait for something to read */
    ret = poll(fds, 1, -1);
    printf("poll returned %ld\n", ret);

    /* read some */
    ret = readv(client_sockfd, iov, 2);
    printf("read %ld\n", ret);

    /* wait for something to read */
    ret = poll(fds, 1, -1);
    printf("poll returned %ld\n", ret);

    /* read some */
    ret = readv(client_sockfd, iov+1, 1);
    /* ret = read(client_sockfd, iov[1].iov_base, iov[1].iov_len); */
    printf("readv read %ld out of %ld\n", ret, iov[1].iov_len);
    if (ret <= 0){
        printf("FAILED: readv read nothing after poll returned POLLIN\n");
        exit(-1);
    }
        
    printf("server exiting\n");

    ret = read(client_sockfd, array, 1);
    printf("read = %ld\n", ret);

    //close(client_sockfd);
    ret = close(client_sockfd);
        
    return 0;
}
