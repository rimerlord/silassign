#include <stdio.h>

#include <time.h>       // for clock()

#include <netdb.h>      // network database management
#include <netinet/in.h> // internet address family

#include <unistd.h>     // for fork and pid_t
/*
 * @file    A secure bidding system
 *          consists of 2 clients(2 bidders, 2 authorised viewers)
 *          and 1 server. See README.md for details
 */

int verify_key(sock_fd)
{
	/* verify code here */
	return 1;
}
void kxchg(int sock_fd)
{
	return 1;
		
}

double time_in_sec()
{
	return clock()*1.0/CLOCKS_PER_SEC;
}

char* key()
{
	return "server";
}

int is_view_or_bid(char *buffer)
{
	/* returns 1 if bidder identified */
	return 1;
}

int doprocessing(int sock_fd)
{
	int n;
	char buffer_in[256], buffer_out[256];
	memset(buffer_in, 0, sizeof(buffer_in));
	memset(buffer_out, 0, sizeof(buffer_out));

	/* read into buffer from socket file descriptor*/
	n = read(sock_fd, buffer, sizeof(buffer));

	if(n<0)
	{
		perror("Error reading from client socket");
		exit(EXIT_FAILURE);
	}

	if(is_view_or_bid(buffer_in))
	{
		/* write buffer to the file */
		if(time_in_sec() < 20)
		{
			n = make_bid(buffer_in, strlen(buffer_in));
			return n;
		}

	}

	else
	{
		/* read bids from file and write back to client */
		if(time_in_sec()>30)
		{
			get_bids(&buffer_out);
			n = write(sock_fd, buffer_out);
			return n;
		}
	}
}

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
		exit(EXIT_FAILURE);
	}

	/* Initialise socket structure */
	serv_addr = memset((char *)&serv_add, 0, sizeof(serv_add)); // fill with zeros
	portno = 5001;
	serv_addr.sin_family        = AF_INET;
	serv_addr.sin_addr.s_addr   = INADDR_ANY;
	serv_addr.sin_port          = htons(portno);

	/* Binding address to socket file descriptor */
	if(bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1)
	{
		perror("Error binding name to socket");
		exit(EXIT_FAILURE);
	}

	/* invoking the program for key exchange */
	if( (argc==2) && (strcmp(argv[1], "kxchg")) )
	{
		kxchg(serv_fd);
	}
	
	/* Now start listening for the clients, here process will
 	* go in sleep mode and will wait for the incoming connection
 	*/
	listen(serv_fd,, 5);
	clilen = sizeof(cli_addr);

	while(1)
	{
	/* Accept actual connection from the client */
	cli_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

	if(cli_fd < 0)
	{
		perror("Erron in accepting client");
		exit(EXIT_FAILURE);
	}

	/* Create child process */
	pid = fork();

	if(pid<0)
	{
		perror("Error in creating child process");
		exit(EXIT_FAILURE);
	}

	if(pid==0)
	{
		/* This is the child process*/
		close(serv_fd);
		doprocessing(cli_fd);
		exit(EXIT_SUCCESS);
	}

	else
		close(cli_fd);
	}


}



