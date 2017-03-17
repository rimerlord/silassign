#include "utils.h"

#include <openssl/dh.h>
#include <openssl/pem.h>
#include <openssl/bn.h> // for BIGNUM    

#include <netdb.h>      // network database management
#include <netinet/in.h> // internet address family
#include <unistd.h>     // for fork and pid_t

#include <stdio.h>      // for perror

/*int make_bid()*/
/*{*/
/*    return 1;*/
/*}*/

/*void get_bids()*/
/*{*/
/*    */
/*}*/


/*int verify_key(sock_fd)*/
/*{*/
/*	/**/
/*	 * verify code here */
///*	 */*/
/*	return 1;*/
/*}*/


void kxchg(int sock_fd)
{
    /*
     * read parameters from the file 
     */ 
	FILE *fp = fopen("dh1024.pem", "r");
	if(fp==NULL)
	{
	    perror("dh1024.pem not found, is it open?");
	}
	/*
	 * extract dh->p and dh->g
	 */
	DH* dh1, dh2 = PEM_read_DHparams(fp, NULL, NULL, NULL);
	/*
	 * generate dh->priv_key, dh->pub_key
	 * verify success
	 */
	int n = DH_generate_key(dh1);
	if(n==0)
	{
	    perror("Error in generating key");
	    exit(0);
	}
	/*
	 * calculate no.of bytes in dh->pub_key
	 * create a buffer of that size
	 * convert in binary format
	 * send the public key over socket
	 * verify success
	 */
	int size = BN_num_bytes(&dh1->pub_key);
	unsigned char *buffer = (unsigned char*)malloc(size*sizeof(unsigned char*));
	size = BN_bn2bin(&dh1->pub_key, buffer);
	n = write(sock_fd, buffer, size);
	if(n<0)
	{
	    perror("Error in sending public key");
	    exit(0);
	}
	else
	{
	    /*
	     * read public key from second person
	     * verify success
	     */
	     memset(buffer, 0, sizeof(buffer));
	     n = read(sock_fd, buffer, sizeof(buffer));
	     if(n<0)
	     {
	         perror("Error in reading public key");
	         exit(0);
	     }
	     else
	     {
	         /*
	          * convert binary to BIGNUM format
	          * compute shared secret
	          * verify success
	          */
	          BN_bin2bn(buffer, size, &dh2->pub_key);
	          &dh2->pub_key = BN_bin2bn(buffer, size, NULL);
	          unsigned char *key = (unsigned char *)malloc(DH_size(dh1)*sizeof(unsigned char*));
	          n = DH_compute_key(key, dh2->pub_key, dh1);
	          if(n<0)
	          {
	              perror("Error on computing shared secret");
	              exit(0);
	          }
	     }  
	}
}

/*double time_in_sec()*/
/*{*/
/*	return clock()*1.0/CLOCKS_PER_SEC;*/
/*}*/

/*char* key()*/
/*{*/
/*	return "server";*/
/*}*/

/*int is_view_or_bid(char *buffer)*/
/*{*/
///*	/* returns 1 if bidder identified */*/
/*	return 1;*/
/*}*/

/*int doprocessing(int sock_fd)*/
/*{*/
/*	int n;*/
///*	/* create buffers */*/
/*	char *buffer_in = (char *)malloc(256*sizeof(char));*/
/*	char *buffer_out = (char *)malloc(256*sizeof(char));*/
/*	*/
///*	/*fill buffers with zeroes*/*/
/*	buffer_in = memset(buffer_in, 0, sizeof(buffer_in));*/
/*	buffer_out = memset(buffer_out, 0, sizeof(buffer_out));*/

///*	/* read into buffer_in from socket file descriptor*/*/
/*	n = read(sock_fd, buffer_in, sizeof(buffer_in));*/

/*	if(n<0)*/
/*	{*/
/*		perror("Error reading from client socket");*/
/*		exit(0);*/
/*	}*/

/*	if(is_view_or_bid(buffer_in))*/
/*	{*/
///*		/* write buffer to the file */*/
/*		if(time_in_sec() < 20)*/
/*		{*/
/*			n = make_bid(buffer_in, strlen(buffer_in));*/
/*			return n;*/
/*		}*/

/*	}*/

/*	else*/
/*	{*/
///*		/* read bids from file and write back to client */*/
/*		if(time_in_sec()>30)*/
/*		{*/
/*			get_bids(&buffer_out);*/
/*			n = write(sock_fd, buffer_out, sizeof(buffer_out));*/
/*			return n;*/
/*		}*/
/*	}*/
/*}*/

