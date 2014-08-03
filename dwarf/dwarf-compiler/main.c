#include <stdlib.h>

#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "codegen.h"
 

int main( int argc, char** argv )
{
	token_list_t list;
	bytecode_builder_t bc;
	ast_t* tree; 

	list = tokenize( "x := 4; if (x) then y:=2 else z := 5" ); 
	tree = parse( list ); 
	bc = generate_from_ast( tree ); 

	puts("");
	bytecode_prettyprint( bc.first );
	//token_list_destroy( &list );
	ast_free( tree );
	getchar();
}


