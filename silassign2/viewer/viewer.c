/*@file A viewer is a client that sends a string "<client_name>"
 *      the server determines whether the viewer is supposed to view the 
 *      bid or not and returns the info on whether the bid was successfully
 *      stored or not.
 */

#include <stdio.h>

#include <netdb.h>      // network database management
#include <netinet/in.h> // internet address

#include <string.h>     // memcpy() and memset()

int send_bid(int dest_fd)
{
    /*@brief makes a request to the server to view the bids and awaits bids sent in reply
     * If the request is unsuccessful, prints the reason for failure and returns -1,
     * returns 0 in case of success.
     **/

    char name[] = "client1";
    //char bid_val[] = "23.34";
    //char *bid;
    char buffer[256];
    //sprintf(bid, "%s %s", name, bid_val);
    int n = write(dest_fd, name, strlen(name));

    if(n<0)
    {
        perror("Couldn't write to server");
        return n;
    }

    else
    {
        /* await reply from server */
        n = read(dest_fd, buffer, sizeof(buffer));
        if(n<0)
        {
            perror("Couldn't read from server");
            return n;
        }

        else
            if(!(strcmp(buffer, "n")))
                printf("The bids are:\n%s", buffer);
                return 1;
            else
            {
                printf("Request unsuccessful");
                return -1;
            }
    }
}

int main(int argc, char *argv[])
{
    int serv_fd, n, port_no;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    if(argc<3)
    {
        printf("Usage %s hostname port_no", argv[0]);
        exit(EXIT_FAILURE);
    }

    else
    {
        port_no = atoi(argv[2]);
        server = gethostbyname(argv[1]);
    }
    
    /* create a socket */
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if( serv_fd<0 )
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    if(server == NULL)
    {
        perror("Error, no such hostname");
        exit(EXIT_FAILURE);
    }

    /* fill serv_addr with zeros */
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->serv_addr.sin_port = htons(port_no));

    /* connect to the server */
    if(connect(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        perror("Couldn't connect to the socket");
        exit(EXIT_FAILURE);
    }

    n = view_bid(sock_fd);
    if(n<0)
    {
        perror("Couldn't make a bid");
        exit(EXIT_FAILURE);
    }

    return 0;


}



