#include <stdlib.h>
#include "common.h"
#include "lexer.h"
#include "AST.h"
#include "parser.h"


/*
***Dwarf Grammar***

statements := statement | statement ";" statements 

statement := "{" statements "}" | assignment | if | while | print
assignment := IDENT "=" expr
if := "if" "(" expr ")" "then" statement "else" statement
while := "while" "(" expr ")" statement

expr := expr1 "+" expr | expr1 "-" expr | expr1
expr1 := atom "*" expr1 | atom "/" expr1 | atom
atom := "(" expr ")" | NUMBER

{ statement }
statement ; statement


*/  

static void error_if_null( ast_t* node, token_list_t** stream, const char* const program_code ) {
	size_t pos;
	if ( node != NULL ) return;
	pos = (**stream).token.start_position ;
	fprintf( stderr, "Error while parsing program at position %d : \n\n%.20s\n", pos, program_code + pos);
	exit( EXIT_FAILURE );
}

static ast_t* statements( token_list_t** stream, const char* const code  );
static ast_t* statement( token_list_t** stream, const char* const code  );
static ast_t* assignment( token_list_t** stream, const char* const code  );
static ast_t* if_then_else( token_list_t** stream, const char* const code  );
static ast_t* while_cond( token_list_t** stream, const char* const code  );
static ast_t* expression( token_list_t** stream, const char* const code  );
static ast_t* expression1( token_list_t** stream, const char* const code  );
static ast_t* atom( token_list_t** stream, const char* const code ); 

/* Helper methods */
static bool_t is_EOF(  token_list_t** stream ) { 
	return (**stream).token.type == TOK_EOF;
}

static bool_t advance( token_list_t** stream ) { 
	if ( ! is_EOF( stream ) ) {
		(*stream) = (**stream).next;
		return TRUE;
	}
	else return FALSE;
} 
static token_t* accept( token_list_t** stream, token_type_t token_type ) 
{
	token_t* token;
	if ( (**stream).token.type == token_type)
	{
		token = &( (**stream).token );
		advance( stream );
		return token;
	}
	else return NULL;
} 


/* Recursive-descent parser */
static ast_t* expression( token_list_t** stream, const char* const code ) {
	ast_t* lhs, *rhs;
	lhs = expression1( stream, code );
	error_if_null( lhs, stream, code );
	if ( accept( stream, TOK_PLUS ) ) {
		rhs = expression( stream, code );		error_if_null( rhs, stream, code );
		return ast_plus( lhs, rhs );
	}
	else if ( accept( stream, TOK_MINUS ) ) {
		rhs = expression( stream, code );		error_if_null( rhs, stream, code );
		return ast_plus( lhs, rhs );
	}
	else return lhs;

}

static ast_t* expression1( token_list_t** stream, const char* const code  ) {
	ast_t* lhs, *rhs;
	lhs = atom( stream, code );
	error_if_null( lhs, stream, code );
	if ( accept( stream, TOK_MULT ) ) {
		rhs = expression1( stream, code );		error_if_null( rhs, stream, code );
		return ast_mult( lhs, rhs );
	}
	else if ( accept( stream, TOK_DIVIDE ) ) {
		rhs = expression1( stream, code ); 		error_if_null( rhs, stream, code );
		return ast_div( lhs, rhs );
	}
	else return lhs;
}


static ast_t* atom( token_list_t** stream, const char* const code  ) {
	token_t* token = NULL;
	ast_t* expr = NULL;
	if( token = accept( stream, TOK_NUM )) 
		return ast_number( token->data.integer );
	else if ( token = accept( stream, TOK_IDENT ) )
		return ast_identifier( token->data.ident ) ;
	else if ( accept( stream, TOK_LPAR )  
		&& ( expr = expression( stream, code ) ) 
		&& accept( stream, TOK_RPAR ) )
		return expr;
	
	error_if_null( expr, stream, code );  
	return NULL;
}



static ast_t* statements( token_list_t** stream, const char* const code  ){
	ast_t* lhs, *rhs;
	lhs = statement( stream, code );
	if ( accept( stream, TOK_SEMICOLON ) ) {
		rhs = statements( stream, code );		
		if ( rhs == NULL ) return lhs; else return ast_seq( lhs, rhs );
	} 
	else return lhs;
}

static ast_t* statement( token_list_t** stream, const char* const code  )
{
	ast_t* result;

	if ( result = assignment( stream, code ) ) return result;
	else if ( accept( stream, TOK_LBRACE ) 
		&& ( result = statements( stream, code ) ) 
		&& accept( stream, TOK_RBRACE ) ) return result;
	else if ( result = if_then_else( stream, code ) ) return result;
	else if ( result = while_cond( stream, code ) ) return result;
	error_if_null( result, stream, code );
	return NULL;
}


static ast_t* assignment( token_list_t** stream, const char* const code  ) {
	ast_t* expr;
	token_t* ident;
	if ( ( ident = accept( stream, TOK_IDENT ) )
		&& accept( stream, TOK_ASSIGN ) 
		&& ( expr = expression( stream, code ) ) )
		return ast_assignment( ast_identifier( ident->data.ident ), expr );
	else return NULL;	
} 

static ast_t* if_then_else( token_list_t** stream, const char* const code  ) {
	ast_t* cond = NULL;
	ast_t* yes = NULL;
	ast_t* no = NULL;

	if ( accept( stream, TOK_IF ) && 
		accept( stream, TOK_LPAR )  &&
		(cond = expression( stream, code )) &&
		accept( stream, TOK_RPAR ) && 
		accept( stream, TOK_THEN )  &&
		(yes = statement( stream, code )) && 
		accept( stream, TOK_ELSE ) &&
		(no = statement( stream, code )) )
		return ast_if_then_else( cond, yes, no );

	ast_free( cond );
	ast_free( yes );
	ast_free( no );
	return NULL;
}

static ast_t* while_cond( token_list_t** stream, const char* const code  ) {
	ast_t* cond = NULL;
	ast_t* body= NULL;
	if ( accept( stream, TOK_WHILE ) && 
		accept( stream, TOK_LPAR )  &&
		(cond = expression( stream, code )) &&
		accept( stream, TOK_RPAR ) && 
		(body = statement( stream, code )) )
		return ast_while( cond, body);

	ast_free( cond );
	ast_free( body );
	return NULL;
}

/* the main parsing method */
ast_t* parse( token_list_t* list, const char* const code  ) {
	ast_t* tree;
	token_list_t* stream = list;
	tree = statements( &stream, code ); 
	return tree;
}
