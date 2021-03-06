#include "linkedList.h"



/************************************************************
 *
 * initializeNode() allocates memory for a new node of
 * Linked List and initializes his contents to NULL
 *
 ************************************************************/
	
struct node *initializeNode(){

 
	    struct node *new = (struct node *)malloc( sizeof(struct node) );

	    memset( new->username, '\0', 20 );
	    memset( new->password, '\0', 15 );

	    new -> next = NULL;
	    new -> last = NULL;
	    
return new;
}

/************************************************************
 *
 * initializeMail_Node() allocates memory for a new node of
 * Linked List and initializes his contents to NULL
 *
 ************************************************************/
	    
struct mail_node *initializeMail_Node() {
  

    struct mail_node *new = (struct mail_node *)malloc( sizeof(struct mail_node) );

	    new -> mail_size = 0;
	    new -> mail_num  = 0;
	    
	    new -> mail = (char *)malloc( 1024 * sizeof(char) );
	    new -> from = (char *)malloc( 64   * sizeof(char) );
	    new -> next = NULL;
	    new -> last = NULL;
	    
	    memset( new->mail, '\0', 1024 );
	    memset( new->from, '\0', 64   );
    
    
return new;
}


/*******************************************************************
 *
 * insertList() insert a new node with username & password member
 * that are given as arguments and return a pointer to the 
 * first node of the linked list.
 *
 *******************************************************************/

struct node *insertList( struct node *headOfList, char *name, char *pass ) {

      
		struct node * new  = initializeNode();
		struct node * tail = NULL;

		new -> next = NULL;					/* modifies the new node of Linked List */
		memcpy( new->username, name, 20 );
		memcpy( new->password, pass, 15 );


		tail = findTailList( headOfList );	/* find and return a pointer to the Tail of Linked List */


		if( tail == NULL ) {			/* if new is the head of Linked List */
		
			new -> last = NULL;
			return new;
		}
		else {
			tail->next = new;
			new ->last = tail;
		}
		
return headOfList;
}


/********************************************************
 *
 * insert_Mail_List() insert a new node to the list
 * And return a pointer to the head of Linked List
 *
 ********************************************************/

struct mail_node *insert_Mail_List( struct mail_node *headOfList, char *mail, char *from, int num ) {
  
		struct mail_node * new  = initializeMail_Node();
		struct mail_node * tail = NULL;


				new -> next = NULL;							/* modifies the new node of Linked List */
				new -> mail_num = num;
				new -> mail_size = strlen(mail);
				
				memcpy( new->mail, mail, strlen(mail) );
				memcpy( new->from, from, strlen(from) );

				tail = findTail_Mail_List( headOfList );	/* find and return a pointer to the Tail of Linked List */


				if( tail == NULL ) {						/* if new is the head of Linked List */
				
					new -> last = NULL;
					return new;
				}
				else {
					tail->next = new;
					new ->last = tail;
				}


return headOfList;
}


/***************************************************************
 *
 * findTailList() takes as argument a pointer to the head of
 * Linked List and returns an pointer to the last node (tail)
 * of List.
 *
 ***************************************************************/ 

struct node *findTailList( struct node *headOfList ) {

struct node *tail = headOfList;


		if( tail == NULL )
			return tail;

		while( tail->next != NULL )
			tail = tail->next;


return tail;
}


/*******************************************************************
 *
 * findTail_Mail_List() takes as argument a pointer to the head of
 * Linked List and returns an pointer to the last node (tail)
 * of List.
 *
 *******************************************************************/

struct mail_node *findTail_Mail_List( struct mail_node *headOfList ) {

struct mail_node *tail = headOfList;


		if( tail == NULL )
			return tail;

		while( tail->next != NULL )
			tail = tail->next;


return tail;
}



void printList( struct node *headOfList ) {

	struct node *temp = headOfList;


	while ( temp != NULL ) {

			printf( "%s ", temp->username );
			printf( "%s",  temp->password );
			printf( "-> " );

			temp = temp->next;
	}

printf("NULL\n\n");
}



void print_Mail_List( struct mail_node *headOfList ) {

	struct mail_node *temp = headOfList;


	while ( temp != NULL ) {

			printf("%s | ", temp->from );
			printf( "%s " , temp->mail );
			printf( "-> " );

			temp = temp->next;
	}

printf("NULL\n\n");
}


/*********************************************************************
 *
 * searchList() takes as argument a pointer to the head of the List
 * and a search-key and linearly searches the list for it and
 * returns a pointer to the node where key found or NULL if key
 * doesn't found. 
 * Also takes one more argument ( sw_key ) that shows us what client 
 * wants to search username or password.
 *
 **********************************************************************/ 

struct node *searchList( struct node *headOfList, char *buff , int sw_key ) {


	struct node *temp = headOfList;


		switch( (int)sw_key ) {
				
			case 1:
			
					while( temp != NULL ) {

						if ( memcmp( temp->username ,buff , strlen(temp->username)) != 0 )
							temp = temp-> next;
						else
							return temp;
					}
					break;
			
			
			case 2:

					while( temp != NULL ) {

						if ( memcmp( temp->password ,buff , strlen(temp->password)) != 0 )
							temp = temp-> next;
						else
							return temp;
					}
					break;
					
		}
	
	
return NULL;
}		


/*******************************************************************************
 *
 * search_Mail_List() takes as argument a pointer to the head of the List
 * and a search-key and linearly searches the list for it and
 * returns a pointer to the node where key found or NULL if key
 * doesn't found. 
 * 
 * *****************************************************************************/

struct mail_node *search_Mail_List( struct mail_node *headOfList, int num ) {	


	struct mail_node *temp = headOfList;
	
	
	while( temp != NULL ) {

			if ( temp->mail_num != num )
				temp = temp-> next;
			else
				return temp;
	}


return NULL;
}


/************************************************************************
 * 
 * deleteList() takes as argument a pointer to the head of Linked List
 * and search-key to delete it and delete node whitch key matches with
 * search-key given and then repair the List. Finally returns a pointer 
 * to the head of modified List.
 *
 ************************************************************************/

struct mail_node * delete_Mail_List( struct mail_node *firstNode, int num ) {

	struct mail_node *temp1 = firstNode;
	struct mail_node *temp2 = firstNode;

	temp1 = search_Mail_List( temp1, (int)num );

	if( temp1 == NULL ) {

		printf("\nMail #%d does not exists\n", num );
	}
	else if( temp1 ->last == NULL ) {

		firstNode = temp1->next;
		firstNode -> last = NULL;

		free( temp1 );
	}
	else if( temp1 -> next == NULL ) {

		temp2         = temp1 -> last;
		temp2 -> next = NULL;

		free(temp1); 
	}
	else {		

		temp2 = temp1->last;
		temp1 = temp1->next;

		free(temp1->last);

		temp2->next = temp1;
		temp1->last = temp2;
	}

return firstNode;

}
	
	



