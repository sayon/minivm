#ifndef TOKEN_LIST
#define TOKEN_LIST

#include "tokens.h" 

typedef struct token_list_t {
	token_t token;
	struct token_list_t* next;
} token_list_t;
 

token_list_t* token_list_create( token_t* token );
void token_list_free( token_list_t* list );
void token_list_foreach( token_list_t* list, void (action)(token_list_t*) );

#endif