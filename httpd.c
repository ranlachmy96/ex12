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
#include <fcntl.h>
#define MAXCONNECTION 100
#define SA struct sockaddr
static char* buf;
static int listenfd, clients[MAXCONNECTION];
typedef struct { char *name, *value; } header_t;
static header_t reqhdr[17] = { {"\0", "\0"} };
static int clientfd;
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
        clients[slot] = accept(listenfd, (struct sockaddr *)&clientaddr, &addrlen);
        
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

void startServer(const char *PORT)
{
 	    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, PORT, &hints, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        int option = 1;
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}

void respond(int n)
{
    int rcvd, fd, bytes_read;
    char *ptr;

    buf = malloc(65535);
    rcvd=recv(clients[n], buf, 65535, 0);

    if (rcvd<0)    // receive error
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // receive socket closed
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // message received
    {
        buf[rcvd] = '\0';

        method = strtok(buf,  " \t\r\n");
        uri    = strtok(NULL, " \t");
        prot   = strtok(NULL, " \t\r\n"); 

        fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);
        
        if (qs = strchr(uri, '?'))
        {
            *qs++ = '\0'; //split URI
        } else {
            qs = uri - 1; //use an empty string
        }

        header_t *h = reqhdr;
        char *t, *t2;
        while(h < reqhdr+16) {
            char *k,*v,*t;
            k = strtok(NULL, "\r\n: \t"); if (!k) break;
            v = strtok(NULL, "\r\n");     while(*v && *v==' ') v++;
            h->name  = k;
            h->value = v;
            h++;
            fprintf(stderr, "[H] %s: %s\n", k, v);
            t = v + 1 + strlen(v);
            if (t[1] == '\r' && t[2] == '\n') break;
        }
        t++; // now the *t shall be the beginning of user payload
        t2 = request_header("Content-Length"); // and the related header if there is  
        payload = t;
        payload_size = t2 ? atol(t2) : (rcvd-(t-buf));

        // bind clientfd to stdout, making it easier to write
        clientfd = clients[n];
        dup2(clientfd, STDOUT_FILENO);
        close(clientfd);

        // call router
        route();

        // tidy up
        fflush(stdout);
        shutdown(STDOUT_FILENO, SHUT_WR);
        close(STDOUT_FILENO);
    }

    //Closing SOCKET
    shutdown(clientfd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clientfd);
    clients[n]=-1;
}
