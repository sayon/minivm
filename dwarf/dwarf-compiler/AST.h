#include "common.h"
#include "tokens.h"

#ifndef AST_H
#define AST_H

typedef enum {
	AST_IF,
	AST_WHILE,
	AST_SKIP,
	AST_ASSIGN,
	AST_NUMBER,
	AST_PLUS,
	AST_MINUS,
	AST_MULT,
	AST_DIVIDE,
	AST_SEQ,
	AST_IDENTIFIER,
	AST_PRINT
} ast_node_type_t;



typedef struct { 
	struct ast_t* first;
	struct ast_t* second;
} ast_seq_t;

typedef struct { 
	struct ast_t* cond;
	struct ast_t* yes;
	struct ast_t* no;
} ast_if_t;

typedef struct { 
	struct ast_t* cond;
	struct ast_t* body;
} ast_while_t; 

typedef struct { 
	char name[ IDENTIFIER_MAX_LENGTH ]; 
} ast_identifier_t;

typedef struct { 
	struct ast_t* identifier;
	struct ast_t* value;
} ast_assign_t;

typedef struct { 
	struct ast_t* left;
	struct ast_t* right;
} ast_binop_t;

typedef struct { 
	struct ast_t* expr;
} ast_unop_t;



typedef struct ast_t {  
	ast_node_type_t type;
	union {
		lang_int_t as_number;
		ast_identifier_t as_identifier;
		ast_seq_t  as_seq;
		ast_if_t   as_if;
		ast_while_t as_while;
		ast_assign_t as_assign;
		ast_binop_t as_binop;
		ast_unop_t as_unop;
	} attributes;
} ast_t;


/* Constructors for different node types */

ast_t* ast_number( lang_int_t num );
ast_t* ast_plus( ast_t* left, ast_t* right );
ast_t* ast_minus( ast_t* left, ast_t* right );
ast_t* ast_mult( ast_t* left, ast_t* right );
ast_t* ast_div( ast_t* left, ast_t* right );
ast_t* ast_assignment( ast_t* ident, ast_t* expr );
ast_t* ast_seq( ast_t* left, ast_t* right );
ast_t* ast_identifier( char* name );
ast_t* ast_if_then_else( ast_t* cond, ast_t* yes, ast_t* no );
ast_t* ast_while( ast_t* cond, ast_t* body );

void ast_free( ast_t* node );
void ast_print( ast_t* node, FILE* out );

#endif 