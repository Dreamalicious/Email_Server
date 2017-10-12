
#include "server_global_header.h"
#include "server_services.h"
#include "linkedList.h"


void mail_server( int port) {


/********************************
 * Local Variables Declaration	*
 ********************************/


	struct sockaddr_in sin;
	struct sockaddr_in client_addr;

	int pid;
	

/********************************************************
 *                                                      *
 * Get a socket to work with. This socket will be	    *
 * in the INET domain, and will be a stream socket.	    *
 *                                                      *
 ********************************************************/


	if ( (sd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ) < 0) {

		perror("server: socket's failure" );
		exit(1);
	}


/***********************************************
 * 
 * Create the address we will be binding to.
 * 
 ***********************************************/

	memset(&sin, 0, sizeof(sin));

	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);


/**********************************************************
 * 
 * Try to bind the address to the socket. We unlink
 * the name first so that the bind won't fail.
 *
 * The third argument indicates the "length" of the
 * structure, not just the length of the socket name.
 * 
 **********************************************************/


	unlink(SOCK_NAME);



	if ( bind( sd_server, (struct sockaddr *)&sin, sizeof( sin ) ) < 0) {

		perror("server: failed to bind.\n" );
		exit(1);
	}

/**************************
 * 
 * Listen on the socket.
 * 
 **************************/


	if ( listen(sd_server, 1) < 0 ) {

		perror("server: fails to listen.\n" );
		exit(1);
	}


/*******************************************
 * 
 *  Create a linked list that contains the 
 *  username and the password of each user,
 *  on each node.
 * 
 * ******************************/

	struct node *user_list = listOFusers();


	while(1) {

				printf("Wating for a client request...\n");
	

	
/************************************************
 * 
 * Accept connections. When we accept one, ns
 * will be connected to the client. client_addr
 * will contain the address of the client.
 * 
 ************************************************/

		memset(&client_addr, 0, sizeof(client_addr));

		int x = 0;
		x = sizeof(client_addr);
	
		if ( (ns = accept( sd_server, (struct sockaddr *) &client_addr, (socklen_t *) &x ) ) < 0 ) {
	
			perror("server: didn't accept" );
			exit(1);
		}
		else {
			printf("Connection accepted!\n");	
		}
	
		

/**********************************************************
 * 
 * Creates a new procedure for every new client's request. 
 * 
 **********************************************************/

		pid = fork();


		if( pid == 0 ) {
			
				server_Menu( user_list );
			
		}
		

	}
	


exit(0);
}



/******************************************************************
 * 
 * server_Menu() takes as argument a pointer points to the first
 * node of a linked list that contains all usernames & passwords
 * of users. 
 * It takes messages from client and calls the function needed to 
 * provide server's service. 
 * 
 * ****************************************************************/

void server_Menu( struct node *head ) {
	
	
	int choice = 0;

	int *flag  = (int *)malloc( sizeof(int) );				/* This flag becomes 1 if user give us   */
		*flag  = 0;											/* right its username and 2 if give us   */
															/* both username and password.           */
	
	struct mail_node *firstNode = NULL;
	char *theName = (char *)malloc( 32 * sizeof(char) );	/* In this variable we store the username */
															/* and make it available to all provide_  */
															/* functions by send it as argument       */
	
	memset( theName, '\0', 32 );
	
	
		
		
	while( choice != 8 ) {						/* Loop executes while client does not want to logout. */
		
		
						if( read( ns, &choice, sizeof(int) ) < 0 ) {	/* Read client's choice */
							
							perror("Server : Menu : Message failed to received...\n");
						}


		
						switch( choice ) {
							
							case 1: 
									create_Account( head );				/* No needed service provided     */
									break;								/* Actually client cannot call it */
							case 2:
									provide_name( head, theName, flag );
									break;
							case 3:
									provide_pass( head, flag );
									
									if( *flag == 2 ) {					/* If user login successfully */
										
												strcat( theName , ".txt" );			/* Contais user's filename */
												firstNode = listOFmails( theName );	/* Look at server_services.c */
									}
									break;
							case 4:
									provide_stat( firstNode );
									break;
							case 5:
									provide_list( firstNode );
									break;
							case 6:
									provide_dele( firstNode );
									break;
							case 7:
									provide_retr( firstNode );
									break;
							case 8:	
									back_up( firstNode, theName );
									printf("+OK Client logout...\n\n");
									close(ns);		
									break;

							default:
									break;
		}
		
		
	}
	
	
	free( firstNode );
	free( flag      );
	free( theName   );
	
}


/******************************************************************
 * 
 * listOFusers() opens file User.txt where are store in order
 * usernames and passwords of all users. Then creates a linked
 * list its node of it contains the username & pass.
 * Returns a pointer points to the first element of the list.
 * 
 * ****************************************************************/

struct node *listOFusers() {
	
		
	FILE *server_File = NULL;
	struct node *head = NULL;

	char userNAME[20];
	char passWORD[15];

	int num_of_users = 0;
	long place = 0;

	
	
		head = insertList( NULL, "user", "0" );			/* Creates the first node of the list */
														/* and returns a pointer to the start */

			if( ( server_File = fopen( "Users.txt", "r" ) ) != NULL ) {
			
			
				fscanf( server_File, "%d", &num_of_users );		/* Reads the number of users    */
																/* that have create account     */
																/* and stored in the first line */
																/* of Users.txt                 */
																
				while( num_of_users != 0 ) {
				
				
					place = ftell(server_File);
					fseek ( server_File, place, SEEK_SET); 		/* Places cursor in right place */
										
					memset( userNAME, '\0', 20 );
					fscanf( server_File, "%s", userNAME );		/* Reads the username from file */
					
					
					
					place = ftell(server_File);
					fseek ( server_File, place, SEEK_SET);		/* Places cursor in right place */
									
					memset( passWORD, '\0', 15 );
					fscanf( server_File, "%s", passWORD );		/* Reads the password from file */

								

					insertList( head, userNAME, passWORD );		/* Insert a new node which contains */
																/* the username and the password    */
					
				num_of_users--;
				}
			
		}
		else {
	
			server_File = fopen( "Users.txt", "a" );
			
			fclose(server_File);
		}


			
return head;					/* Returns a pointer points to the first list's node. */

}





/*************************************************
 * 
 *  Function that terminatates child processes.	
 * 
 * ************************************************/

void sig_child_terminate( int sig_a ) {
  
  pid_t pid;
  int stat;
  
  while( pid = waitpid( -1, &stat, WNOHANG ) > 0 ) 
		printf("\nChild with pid = %d terminated\n", pid);	  
  
  
}



/*************************************************
 * 
 *  Function executed when a SIGINT signal is	
 * caught. It is used to terminate the server	
 * cleanly when Ctrl-C is pressed.		
 * 
 * ************************************************/

void sig_exit_server( int sig_b ) {
  
	printf("\n===========================\n");
	printf("\t Server Terminated\n");
	printf("===========================\n");


exit(1);

}
 


int main( int argc, char *argv[] ) {
	
	system("clear");
	
	printf("\n( Type Ctrl^C to terminate Server ).\n\n");
	
		
		signal( SIGCHLD, sig_child_terminate );
		signal( SIGINT, sig_exit_server );
		
	
	mail_server( atoi(argv[1]) );
	
	
	return 0;
}

