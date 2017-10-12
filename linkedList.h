#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include "server_global_header.h"



	/********************
	 * GLOBAL VARIABLES *
	 ********************/


	struct node {			/* Double Linked list's node struct */

        	char username[20];
			char password[15];

        	struct node *last;	/*Points to the last node of list*/
        	struct node *next;	/*Points to the next node of list*/
	};
	
	

	struct mail_node {
	  
		char *mail;				/* Points to the mail message */
		char *from;				/* Points to the sender       */
		
		int mail_num;			/* Contains the number of mail*/
		long mail_size;			/* Contains the size of mail  */
	  
		struct mail_node *last;	/* Points to the last node of the list */
		struct mail_node *next;	/* Points to the next node of the list */
	};	  


	/*********************************
	 * FUNCTION PROTOTYPE DEFINITION *
	 *********************************/


	struct node * initializeNode();
	struct node * searchList    ( struct node *, char * , int );
	struct node * insertList    ( struct node *, char *, char * );
	struct node * findTailList  ( struct node * );



	struct mail_node * initializeMail_Node();
	struct mail_node *insert_Mail_List    ( struct mail_node *, char *, char *, int );
	struct mail_node *findTail_Mail_List  ( struct mail_node * );
	struct mail_node *search_Mail_List    ( struct mail_node * , int );
	struct mail_node *delete_Mail_List    ( struct mail_node *firstNode, int num );

	void print_Mail_List    ( struct mail_node * );
	void          printList ( struct node * );




#endif
