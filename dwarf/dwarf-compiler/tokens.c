#include "tokens.h"
#include <stdlib.h>
 
token_t token_EOF = { TOK_EOF, 0 }; 


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