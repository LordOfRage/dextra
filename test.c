#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t got_signal = 0;

static void my_sig_handler(int signo)
{
    got_signal = 1;
}

int main()
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = &my_sig_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    for (;;) {
        if (got_signal) {
            got_signal = 0;
            printf("Received interrupt signal!\n");
        }
        printf("Doing useful stuff...\n");
        sleep(1); /* Sleep is not only useful, it is essential! */
    }
    return EXIT_SUCCESS;
}
