#include <stdlib.h>

#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "codegen.h"
#include "io.h"

static void token_item_print( token_list_t* item ) {
	token_print( & ( item->token ) );
}

static char* read_file_by_name( char* name ) {
	FILE* input;
	char* contents;
	input = fopen( name, "r" );
	if( input ) {
		contents = read_file( input );
		fclose( input );
		return contents;
	} else return NULL;

}

void usage( void ) {
	fprintf( stderr, "Usage: dwarf-compiler input-file-name output-file-name\n" );
}
int main( int argc, char** argv )
{
	token_list_t* list;
	bytecode_builder_t bc;
	ast_t* tree; 
	char* program; 
	FILE* output;
	char* input_filename = NULL;
	char* output_filename = "out.dwarfvm";

	if (argc != 2 && argc != 3 ) { usage(); return EXIT_FAILURE; }
	
	if ( argc >= 2 ) input_filename = argv[1];
	if ( argc >= 3 ) output_filename = argv[2];

	program = read_file_by_name( input_filename );
	
	if ( program == NULL ) { fprintf( stderr, "Can not read file %s!\n", input_filename ); return EXIT_FAILURE; }

	list = tokenize( program ); 

	puts("Tokens:");
	token_list_foreach( list, token_item_print );
	puts("");
	tree = parse( list, program ); 
	puts("Parsing successful, ast tree: " );
	ast_print( tree, stdout );
	puts( "\nGenerating bytecode:" );
	bc = generate_from_ast( tree ); 

	bytecode_prettyprint( bc.first );

	output = fopen( output_filename, "wb");
	if (output)	
		bytecode_flush( bc.first , output);
	
	else { fprintf( stderr, "Can not write bytecode to file %s!", output_filename);  return EXIT_FAILURE; }

	free( program );
	token_list_free( list );
	ast_free( tree );
	bytecode_free( bc.first ); 

}


