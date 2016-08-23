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

///   CLIENT   

#define LEN (2147483648)

int main(int argc, char *argv[])
{
    printf("This is the client program\n");

    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    ssize_t ret;
    struct iovec iov[2];
    char *array;
    char header[40] = {0};
    struct pollfd fds[1];

    array = malloc(LEN);
    assert(array);
    memset(array, 0, LEN);
            

    if (argc != 3) {
        printf ("usage: client <serverhost> <serverport>\n");
        exit(-1);
    }

    //Create socket for client.
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        perror("Socket create failed.\n") ; 
        return -1 ; 
    } 
	
    //Name the socket as agreed with server.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(atoi(argv[2]));
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
        perror("Error has occurred");
        exit(-1);
    }

    iov[0].iov_base = header;
    iov[0].iov_len = 40;
    iov[1].iov_base = array;
    iov[1].iov_len = LEN;

    fds->fd = sockfd;
    fds->events = POLLOUT;

    /* wait to be able to write */
    ret = poll(fds, 1, -1);
    printf("poll returned %ld\n", ret);

    /* write some */
    ret = writev(sockfd, iov, 2);
    printf("wrote %ld\n", ret);

    do {
        /* wait to be able to write */
        ret = poll(fds, 1, -1);
        printf("poll returned %ld\n", ret);
        if (fds->events & POLLERR) {
            printf("poll error\n");
            exit(-1);
        }
        /* write some */
        ret = writev(sockfd, iov+1, 1);
        /* ret = write(sockfd, iov[1].iov_base, iov[1].iov_len); */
        printf("writev wrote %ld out of %ld\n", ret, iov[1].iov_len);
        if (ret == 0) {
            printf("FAILED writev wrote nothing after poll returned POLLOUT\n");
            //exit(-1);
        }
    }while (ret == 0);
    
    ret = write(sockfd, array, 1);
    printf("write = %ld\n", ret);

    ret = close(sockfd);
    printf("close = %ld\n", ret);

    exit(0);
}
