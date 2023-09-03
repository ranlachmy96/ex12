#include "httpd.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#define MAXCONNECTION 100

static int listen, clients[MAXCONNECTION];
static void startserver(const char *);
void serve_forever(const char *PORT)
{

    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;

    int slot = 0;

    int i;
    for (i = 0; i < MAXCONNECTION; i++)
    {
        clients[i] = -1;
    }

    startServer(PORT);
    signal(SIGCHLD, SIG_IGN);

    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept(listen, (struct sockaddr *)&clientaddr, &addrlen);

        if (clients[slot] < 0)
        {
            perror("no accepts yet");
        }
        else
        {
            if (fork() == 0)
            {
                respond(slot);
                exit(0);
            }
        }
        while (clients[slot] != -1)
        {
            slot = (slot + 1) % MAXCONNECTION;
        }
    }
}

void startServer(const char *port)
{
    struct addrinfo hints,*res, *p;

    memset(&hints,0,sizeof(hints));

}
