#ifndef SERVER_SERVICES_H
#define SERVER_SERVICES_H

#include "linkedList.h"


struct node *listOFusers();


void server_Menu   ( struct node *  );
void create_Account( struct node *  );
void newFile       ( char *, char * );

void provide_name( struct node * , char * , int * );
void provide_pass( struct node * , int * );

void provide_stat( struct mail_node * );
void provide_list( struct mail_node * );
void provide_dele( struct mail_node * );
void provide_retr( struct mail_node * );
void back_up     ( struct mail_node * , char * );

struct mail_node *listOFmails( char * );



#endif
