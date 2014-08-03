#include <stdlib.h> 


#include "AST.h"
#include <string.h>

void ast_visit( ast_t* node, void (before)( ast_t* node ), void (after)( ast_t* node )) {
	if ( node == NULL ) return;

	if ( before != NULL ) before( node );
	switch ( node->type )
	{
	case AST_IF:
		ast_visit( node->attributes.as_if.cond, before, after );
		ast_visit( node->attributes.as_if.yes, before, after );
		ast_visit( node->attributes.as_if.no, before, after ); 
		break;
	case AST_WHILE:
		ast_visit( node->attributes.as_while.cond, before, after );
		ast_visit( node->attributes.as_while.body, before, after ); 
		break;
	case AST_ASSIGN:
		ast_visit( node->attributes.as_assign.value, before, after );
		ast_visit( node->attributes.as_assign.identifier, before, after );
		break;
	case AST_PLUS:
	case AST_MINUS:
	case AST_MULT:
	case AST_DIVIDE:
	case AST_LT: 
	case AST_LE:
	case AST_EQ:
	case AST_GE:
	case AST_GT:
		ast_visit( node->attributes.as_binop.left, before, after );
		ast_visit( node->attributes.as_binop.right, before, after );
		break;
	case AST_SEQ:
		ast_visit( node->attributes.as_seq.first, before, after );
		ast_visit( node->attributes.as_seq.second, before, after );
		break;
	case AST_PRINT:
		ast_visit( node->attributes.as_unop.expr, before, after );
		break;
	case AST_IDENTIFIER:
	case AST_NUMBER:
	case AST_SKIP:
		break;
	default:
		fprintf( stderr, "Can't deallocate memory for such type of AST nodes!" );
	}

	if ( after != NULL ) after( node );
} 


static void node_free( ast_t* node ) { free( node ); }
void ast_free( ast_t* node ) {
	if ( node == NULL ) return;
	else ast_visit( node, NULL, node_free );
} 

void ast_print( ast_t* node, FILE* out ) {
	if ( node == NULL ) { fprintf( stderr, "Pointer to AST is null!" ); return; }
	switch ( node-> type )
	{
	case AST_IF:
		fprintf( out, "if (" );
		ast_print( node->attributes.as_if.cond, out );
		fprintf( out, ") then {" );
		ast_print( node->attributes.as_if.yes, out );
		fprintf( out, "} else {" );
		ast_print( node->attributes.as_if.no, out ); 
		fprintf( out, "}" );
		break;
	case AST_WHILE:
		fprintf( out, "while (" );
		ast_print( node->attributes.as_while.cond, out );
		fprintf( out, ") {" );
		ast_print( node->attributes.as_while.body, out ); 
		fprintf( out, "}" );
		break;
	case AST_ASSIGN:
		ast_print( node->attributes.as_assign.identifier, out );
		fprintf( out, " := " );
		ast_print( node->attributes.as_assign.value, out );
		break;
	case AST_PLUS:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " + " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_MINUS:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " - " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_MULT:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " * " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_DIVIDE:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " / " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_LT: 
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " < " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_LE:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " <= " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_EQ:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " == " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_GE:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " >= " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_GT:
		fprintf( out, "( " );
		ast_print( node->attributes.as_binop.left, out );
		fprintf( out, " > " );
		ast_print( node->attributes.as_binop.right, out );
		fprintf( out, " )" );
		break;
	case AST_SEQ:
		ast_print( node->attributes.as_seq.first, out );
		fprintf( out, " ; " );
		ast_print( node->attributes.as_seq.second, out );
		break;
	case AST_PRINT:
		fprintf( out, "print( " );
		ast_print( node->attributes.as_unop.expr, out );
		fprintf( out, " )" );
		break;
	case AST_NUMBER:
		fprintf( out, LANG_INT_PRINTF_SPECIFICATOR, node->attributes.as_number );
		break;
	case AST_IDENTIFIER:
		fprintf( out, "%s", node->attributes.as_identifier.name );
		break;
	case AST_SKIP:
		fprintf( out, "skip" );
		break;
	default:
		break;
	}
}
