#ifndef    GLOBAL_HEADER_H
#define    GLOBAL_HEADER_H 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include   <sys/wait.h>


#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>





#define 	SOCK_NAME	 "socket"		 /* address to connect */





/***********************************
 * GLOBAL VARIABLES
 ***********************************/


	int sd_server;			/* Server's Socket descriptor */
	int ns;



/***************************************
 * FUNCTION PROTOTYPE DECLARATION
 ***************************************/



	void mail_server( int );
	
	void sig_child_terminate( int );
	void sig_exit_server( int );
	

#endif
