#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <time.h>       // for clock()
#include <string.h>     // for memset()

#include <netdb.h>      // network database management
#include <netinet/in.h> // internet address family

#include <unistd.h>     // for fork and pid_t
#include "utils/utils.h"// for doprocessing, kxchg, make_bid, is_view_or_bid
/*
 * @file    A secure bidding system
 *          consists of 2 clients(2 bidders, 2 authorised viewers)
 *          and 1 server. See README.md for details
 */

int main(int argc, char *argv[])
{
	int serv_fd, cli_fd, portno, n;
	struct sockaddr_in serv_addr, cli_addr;
	pid_t pid;
	
	/* creation of socket */
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(serv_fd < 0)
	{
		perror("Couldn't create socket");
		exit(0);
	}

	/* Initialise socket structure */
	memset((char *)&serv_addr, 0, sizeof(serv_addr)); // fill with zeros
	portno = 5001;
	serv_addr.sin_family        = AF_INET;
	serv_addr.sin_addr.s_addr   = INADDR_ANY;
	serv_addr.sin_port          = htons(portno);

	/* Binding address to socket file descriptor */
	if(bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1)
	{
		perror("Error binding name to socket");
		exit(0);
	}

	/* invoking the program for key exchange */
	if( (argc==2) && (strcmp(argv[1], "kxchg")) )
	{
		kxchg(serv_fd);
	}
	
	/* Now start listening for the clients, here process will
 	* go in sleep mode and will wait for the incoming connection
 	*/
	listen(serv_fd, 5);
	socklen_t clilen = sizeof(cli_addr);
	while(1)
	{
	/* Accept actual connection from the client */
	cli_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &clilen);

	if(cli_fd < 0)
	{
		perror("Erron in accepting client");
		exit(0);
	}

	/* Create child process */
	pid = fork();

	if(pid<0)
	{
		perror("Error in creating child process");
		exit(0);
	}

	if(pid==0)
	{
		/* This is the child process*/
		close(serv_fd);
		doprocessing(cli_fd);
		exit(1);
	}

	else
		close(cli_fd);
	}


}



