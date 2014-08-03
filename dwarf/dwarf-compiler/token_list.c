#include <stdlib.h>
#include "token_list.h"

token_list_t* token_list_create( token_t* token ) {
	token_list_t* item = (token_list_t*) malloc( sizeof( token_list_t ) );
	item->next = NULL;
	item->token = *token;
	return item; 
}

 
void token_list_foreach( token_list_t* list, void (action)(token_list_t*) ) { 
	token_list_t* current = list;
	token_list_t* next;

	if ( list == NULL ) { fprintf( stderr, "token list is null!" ); abort(); return; }

	while ( current != NULL )
	{
		next = current-> next;
		action( current );
		current = next;
	}
}


static void token_list_item_destroy( token_list_t* item ) { free( item ); }
void token_list_free( token_list_t* list ) {
	token_list_foreach( list, token_list_item_destroy );
}