#include <stdlib.h>

#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "codegen.h"
#include "io.h"

static void token_item_print( token_list_t* item ) {
	token_print( & ( item->token ) );
}
int main( int argc, char** argv )
{
	token_list_t* list;
	bytecode_builder_t bc;
	ast_t* tree; 
	char* program;
	FILE* input;

	input = fopen( "program.txt", "r" );
	if (!input) return EXIT_FAILURE;

	program = read_file( input );
	fclose( input );

	list = tokenize( program ); 
	puts("Tokens:");
	token_list_foreach( list, token_item_print );
	puts("");
	tree = parse( list, program ); 
	bc = generate_from_ast( tree ); 

	puts("");
	bytecode_prettyprint( bc.first );
	 
	free( program );
	token_list_free( list );
	ast_free( tree );
	//free bytecode?

}


