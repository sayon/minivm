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
		break;

	case AST_PLUS:
	case AST_MINUS:
	case AST_MULT:
	case AST_DIVIDE:
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

	after( node );
} 


static void ast_node_free( ast_t* node ) {
	free( node );
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

void ast_free( ast_t* node ) {
	if ( node != NULL )	ast_visit( node, NULL, ast_node_free );
}


//helper method, inaccessible from outside.
static ast_t* ast_binop( ast_t* left, ast_t* right, ast_node_type_t operation_type )
{ 
	ast_t* node = (ast_t*) malloc( sizeof( ast_t* ) );
	node->type = operation_type;
	node->attributes.as_binop.left = left;
	node->attributes.as_binop.right = right;
	return node;
}


ast_t* ast_number( lang_int_t num ) {
	ast_t* node = (ast_t*) malloc( sizeof( ast_t* ) );
	node->type = AST_NUMBER;
	node->attributes.as_number = num;
	return node;
}


ast_t* ast_plus( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_PLUS );
}

ast_t* ast_minus( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_MINUS );
}

ast_t* ast_mult( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_MULT );
}

ast_t* ast_div( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_DIVIDE );
}


ast_t* ast_seq( ast_t* left, ast_t* right)
{ 
	ast_t* node = (ast_t*) malloc( sizeof( ast_t* ) );
	node->type = AST_SEQ;
	node->attributes.as_binop.left = left;
	node->attributes.as_binop.right = right;
	return node;
}
ast_t* ast_identifier( char* name ) {
	ast_t* node;
	node = (ast_t*) malloc( sizeof( ast_t ) );
	node->type = AST_IDENTIFIER;
	strncpy( node->attributes.as_identifier.name, name, IDENTIFIER_MAX_LENGTH );
	return node;
}

ast_t* ast_if_then_else( ast_t* cond, ast_t* yes, ast_t* no ) {
	ast_t* node;
	node = (ast_t*) malloc( sizeof( ast_t ) );
	node->type = AST_IF;
	node->attributes.as_if.cond = cond;
	node->attributes.as_if.yes = yes;
	node->attributes.as_if.no = no;
	return node;
}

ast_t* ast_while( ast_t* cond, ast_t* body ) {
	ast_t* node;
	node = (ast_t*) malloc( sizeof( ast_t ) );
	node->type = AST_WHILE;
	node->attributes.as_while.cond = cond;
	node->attributes.as_while.body = body;
	return node;
}
ast_t* ast_assignment( ast_t* ident, ast_t* expr ) {
	ast_t* node;
	node = (ast_t*) malloc( sizeof( ast_t ) );
	node->type = AST_ASSIGN; 
	node->attributes.as_assign.identifier = ident;
	node->attributes.as_assign.value = expr;
	return node;
}