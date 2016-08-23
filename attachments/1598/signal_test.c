#include <stdio.h>
#include <signal.h>

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

static void signal_cb(int signum)
{
    printf("got signal %d\n", signum);

    return;
}

int main()
{
    signal(SIGUSR1, signal_cb);

    sleep(100);

    return 0;
}
