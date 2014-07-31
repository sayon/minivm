#include "tokens.h"
#include <stdlib.h>

#define CHECK_LIST_NOT_NULL( lst ) if ( lst == NULL ) { fprintf( stderr, "token list is null!" ); abort(); return; }

token_t token_EOF = { TOK_EOF, 0 };


token_list_item_t* token_list_item_create( token_t token ) {
	token_list_item_t* item = (token_list_item_t*) malloc( sizeof( token_list_item_t ) );
	item->next = NULL;
	item->token = token;
	return item;

}
void token_list_add( token_list_t* list, token_t token ) { 
	token_list_item_t* item;

	CHECK_LIST_NOT_NULL( list );

	item = token_list_item_create( token );

	if ( list-> first == NULL ) 
		list-> first = list-> last = item;
	else  
	{
		list-> last-> next = item;
		list->last = item;
	}
} 

token_list_t token_list_create(void) {
	token_list_t list;
	list.first  = list.last = NULL;
	return list;
}

void token_list_foreach( token_list_t* list, void (action)(token_t*) ) { 
	token_list_item_t* current, *next;

	CHECK_LIST_NOT_NULL( list );

	current = list-> first;
	
	while ( current != NULL )
	{
		next = current-> next;
		action( &(current-> token) );
		current = next;
	}
}

void token_print( token_t* token ) {
	printf(" ");
	switch (token->type)
	{
	case TOK_IDENT: 
		printf( "<%s>", token->data.ident );
		break;
	case TOK_NUM:
		printf( LANG_INT_PRINTF_SPECIFICATOR, token->data.integer );
		break;
	default:
		printf( "%s", token_strings[ token-> type ] );
		break;
	}
	printf(" ");
}