#include "server_services.h"
#include "server_global_header.h"


/*************************************************************
 * 
 * newFile() takes as arguments two strings. The first one 
 * contains the filename that we want to create/open. The 
 * second contains the operation we want to operate ( its 
 * something between "w" , "a" or "r" ).
 * 
 * ***********************************************************/

void newFile( char *filename, char *operation ) {
    
     
FILE *fd = NULL;


       if ( ( fd = fopen( filename, operation ) ) != NULL ) {
            
                printf("File: %s just created.\n", filename);
                
				fclose(fd);
        }
        else {
				perror( "Could not create file\n" );
                exit(1);
		}


}


/******************************************************************************
 * 
 * provide_name() has three arguments. The first one it's a pointer to the 
 * first node of the list that contains usernames. The second is an empty
 * -till now- string in which we put the username that client send to server.
 * The third argument is a flag that we set it in value 1 if user send us a 
 * valid username.
 * 
 * ****************************************************************************/

void provide_name( struct node *head, char *theName, int *flag ) {
	
	struct node *pt = NULL;
	
	char *buff = malloc( 20 );
	memset( buff, '\0', 20 );
	
	read( ns, buff, 20 );				/* Reads username that client sends.   */
	
	pt = searchList( head, buff, 1 );	/* Search list for this username and   */
										/* returns a pointer to the right node */
										/* or a NULL pointer if username is't  */
	if( pt != NULL ) {					/* valid.                              */
		
		write( ns, "OK", 2 );			/* Sends this message to confirm that  */
										/* username is valid.                  */
		*flag = 1;						/* Set flag 1.                         */
		memcpy( theName, buff, strlen(buff) );
	}
	else
		write( ns, "KO", 2 );			/* Sends this message to inform client  */
										/* that he sends us invalid username.   */

free( buff );

}


/***********************************************************************
 * 
 * provide_pass() has two arguments. The first one it's a pointer to the 
 * first node of the list that contains passwords. The second a flag 
 * that we set in value 2 if user send us a valid password AND before
 * that a valid username. 
 * 
 * *********************************************************************/

void provide_pass( struct node *head, int *flag ) {
  
	struct node *pt = NULL;
	
	char *buff = malloc( 15 );
	memset( buff, '\0', 15 );
	
	read( ns, buff, 15 );				/* Reads username that client sends.   */
	
	pt = searchList( head, buff, 2 );	/* Search list for this password and   */
										/* returns a pointer to the right node */
										/* or a NULL pointer if password is't  */
	if( pt != NULL ) {					/* valid.                              */
		
		write( ns, "OK", 2 );			/* Sends this message to confirm that  */
										/* password is valid.                  */
		if( flag )						/* If client has already send a valid  */
			*flag = 2;					/* username and password then set it 2 */

	}
	else
		write( ns, "KO", 2 );			/* Sends this message to inform client  */
										/* that he sends us invalid username.   */

free(buff);

}



/***********************************************************************
 * 
 * provide_stat has one argument. It's a pointer to the first node 
 * of a linked list contains all current user's emails and its info.
 * 
 * *********************************************************************/

void provide_stat( struct mail_node *firstNode ) {
	
	int total_numb = 0;
	int total_size = 0;
	
	struct mail_node *temp = firstNode->next;	/* Points to the first node that contains */
												/* useful information.                    */
	
			while( temp != NULL ) {				/* Visits all list's nodes by order.      */
				
				total_size += temp->mail_size;	/* Informs about the total size of emails.  */
				total_numb++;					/* Informs about the total number of emails */
				
			temp = temp->next;	
			}
	
			write( ns, &total_numb, sizeof(int) );	/* Sends info to client */
			write( ns, &total_size, sizeof(int) );	/* Sends info to client */
			
}


/***********************************************************************
 * 
 * provide_list() has one argument which is a pointer to the start of
 * a linked list that contains the user's emails and  its info.
 * 
 * *********************************************************************/

void provide_list( struct mail_node *firstNode ) {
	
	int total_numb = 0;
	int curr_size  = 0;
	int i;
	
	struct mail_node *temp = firstNode->next;	/* Points to the first node that contains */
												/* useful information.                    */
	
	
			while( temp != NULL ) {		/* Counts the number of user's emails */
										/* by visiting all list's nodes.      */
				total_numb++;
				
			temp = temp->next;	
			}
	
			write( ns, &total_numb, sizeof(int) );	/* Sends info to client */
			
			temp = firstNode->next;				/* Points to the first node that contains */
												/* useful information.                    */
	
			
			for( i=0; i<total_numb; i++ ) {
				
				curr_size = temp->mail_size;			/* Current mail size informs. */
				
				write( ns, &curr_size, sizeof(int) );	/* Sends info to client */
			
			curr_size = 0;								/* Current mail size initialises to zero */
			temp = temp->next;
			}
	
}


/***********************************************************************
 * 
 * provide_dele() has one argument which is a pointer to the start of
 * a linked list that contains the user's emails and  its info.
 *  
 * *********************************************************************/

void provide_dele( struct mail_node *firstNode ) {
	
	
	struct mail_node *temp = NULL;

	int mail_number = 0;
	

		read( ns, &mail_number, sizeof(int) );					/* Reads the sequence number of mail */
																/* that we want to delete            */
		
		temp = delete_Mail_List( firstNode, mail_number );		/* Delete mail from the list                  */
																/* $$ Not from file yet just from the list $$ */
		
		if( temp != NULL )
			write( ns, "OK", 2 );			/* Sends this message to inform client that the operation work */
		else
			write( ns, "KO", 2 );			/* Sends this message to inform client that the operation did'nt work */
		
}


/***********************************************************************
 * 
 * provide_retr() has one argument which is a pointer to the start of
 * a linked list that contains the user's emails and  its info.
 * 
 * *********************************************************************/

void provide_retr( struct mail_node *firstNode ) {
	
	int num = 0;
	int size = 0;
	int i;
	struct mail_node *temp = NULL;
	
	char *mail_messg = malloc( 1024 * sizeof(char) );	/* A buffer used to store the mail message. */
	char *from       = malloc( 64   * sizeof(char) );	/* A buffer used to store the sender person */
	
	memset( mail_messg, '\0', 1024 );
	memset( from      , '\0', 64   );
	
	
	read( ns, &num, sizeof(int) );						/* Reads the number of mail that client want */
														/* to read.                                  */
	temp = firstNode->next;
	
	for( i=1; i<num; i++ )								/* Visit the node that contains the mail message */
		temp = temp->next;
		
	size = temp->mail_size;
	memcpy( mail_messg, temp->mail, 1024);				
	memcpy( from      , temp->from, 64  );				
	
	write( ns, &size, sizeof(int) );					/* Sends info to client */
	write( ns, from,       64     );
	write( ns, mail_messg, 1024   );
	
	
free( mail_messg );										/* Free buffer's memory */
free( from );

}



/***********************************************************************
 * 
 * listOFmails() takes an argument, a string that contains the name
 * of user's filename, where are stored his emails. 
 * This function open this file and then takes its one of emails and
 * put them on a node of a linked list. 
 * Returns a pointer points on the first element of the list.
 * 
 * *********************************************************************/


struct mail_node *listOFmails( char *theName ) {
	
	
	struct mail_node *firstNode = NULL;
	
	FILE *fp = NULL;
	char *mailContents = (char *)malloc( 1024 * sizeof(char) );		/* Variable where we store temporarely an email */
	char *line         = (char *)malloc( 1024 * sizeof(char) );		/* Variable where we store temporarely a file line */
	char *from         = (char *)malloc( 64   * sizeof(char) );		/* Variable where we store the sender.  */
	char *user_file    = (char *)malloc( strlen(theName) + 1 );		/* Variable where we store the filename */

	int seq_number  = 0;		
	int num_ofmails = 0;		
	int temp        = 0;		
	long place      = 0;		
	
	memset( mailContents, '\0', 1024 );
	memset( line        , '\0', 1024 );
	memset( user_file   , '\0', strlen(theName)+1 );
	
	memcpy( user_file, theName, strlen(theName) );					/* Copies the filename to this var */
			
			/* Creates the first node of the list */
			
			firstNode = insert_Mail_List( firstNode, "mail_0", "From <user>@GTmail.com", seq_number );	
			
			
			
			if( (fp = fopen( user_file, "r" ) ) == NULL ) {
				
					printf("\nFile %s didn't open.\n", theName);

			}
			else {
				
					while( fgets( line, 1024, fp ) != NULL ) {		/* Reads file line to line */
						
							place = ftell(fp);
							fseek( fp, place, SEEK_SET );
				
							
							if( memcmp( line, "<<<", 3 ) == 0 ) {	/* When finds this indification */
																	/* means that a new email begin */
								
								memcpy( from, line+3, strlen(line) - 7 );
								
								memset( mailContents, '\0', 1024 );
								memset( line        , '\0', 1024 );

								temp++;
								seq_number++;						/* Counts the sequence number of email */
								
								place = ftell(fp);
								fseek( fp, place, SEEK_SET );
								fgets( line, 1024, fp      );
					
								insert_Mail_List( firstNode, line, from, seq_number );	/* Creates a new node */
								
								memset( line, '\0', 1024 );
								
							}

					}
				
				
			fclose( fp );	
			}
			
			
	free( mailContents );	
	free( user_file    );
	free( from         );
	free( line         );	

	
return firstNode;

}



/***********************************************************************
 * 
 * back_up() has two arguments. The first is a pointer points to the
 * first element of a linked list that contains user's mails and the 
 * second user's filename where mails are used to stored.
 * This function writes the updated mail-list (maybe user dele a mail)
 * back to the file.
 * 
 * *********************************************************************/

void back_up ( struct mail_node *firstNode , char *theName ) {
		
		
	FILE *fp = NULL;
		
	struct mail_node *temp = NULL;	
	
	char *from = (char *)malloc( 64   * sizeof(char) );
	
	long place = 0;
			
						
		if ( ( fp = fopen( theName, "w" ) ) == NULL )
			printf("\nSystem failed to back up.\n");
		else {
						
				temp = firstNode->next;			/* Points to the first node that contains */
												/* useful information.                    */
				
				while( temp != NULL ) {			/* Visit all list's nodes */
					
					memset( from, '\0', 64   );
					memcpy( from, "<<<", 3   );

					strcat( from, temp->from );
					strcat( from, ">>>\n"    );
					
					place = ftell(fp);
					fseek(fp, place, SEEK_SET);
					
					fwrite( from      , sizeof(char), strlen(from)      , fp );
					fwrite( temp->mail, sizeof(char), strlen(temp->mail), fp );
					
				temp = temp->next;	
				}
				
		fclose(fp);
		
		}
		
		printf("\n+OK File <%s> Back up successfully.\n\n", theName );
		
	
	free(from);
}		


/***********************************************************************
 * 
 * Not Used Funcion...
 * 
 * *********************************************************************/

void create_Account( struct node *head ) {
	

	int  check_communication = 0;

	char username[20];
	char password[15];
	
	
		memset( username, '\0', 20 );
		memset( password, '\0', 15 );
		
		
		read( ns, username, 20 );
		read( ns, password, 15 );
		
		check_communication = 1;
		
		write( ns, &check_communication, sizeof(int) );
			

		
		insertList( head, username, password );
		
		newFile( strcat( username, ".txt" ) , "w" );

}




