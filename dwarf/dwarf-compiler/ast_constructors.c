#include <stdlib.h> 


#include "AST.h"
#include <string.h> 
 
static ast_t* ast_binop( ast_t* left, ast_t* right, ast_node_type_t operation_type )
{ 
	ast_t* node = (ast_t*) malloc( sizeof( ast_t ) );
	node->type = operation_type;
	node->attributes.as_binop.left = left;
	node->attributes.as_binop.right = right;
	return node;
} 

ast_t* ast_number( lang_int_t num ) {
	ast_t* node = (ast_t*) malloc( sizeof( ast_t ) );
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

ast_t* ast_lt( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_LT );
}

ast_t* ast_eq( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_EQ );
}

ast_t* ast_gt( ast_t* left, ast_t* right ) {
	return ast_binop( left, right, AST_GT );
}



ast_t* ast_seq( ast_t* left, ast_t* right)
{ 
	ast_t* node = (ast_t*) malloc( sizeof( ast_t ) );
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