#include "lexer.h"
#include "AST.h"

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

ast_t* statements( token_list_item_t** stream );
ast_t* statement( token_list_item_t** stream );
ast_t* assignment( token_list_item_t** stream );
ast_t* if_then_else( token_list_item_t** stream );
ast_t* while_cond( token_list_item_t** stream );
ast_t* expression( token_list_item_t** stream );
ast_t* expression1( token_list_item_t** stream );
ast_t* atom( token_list_item_t** stream ); 

/* Helper methods */
static bool_t is_EOF(  token_list_item_t** stream ) { 
	return (**stream).token.type == TOK_EOF;
}

static bool_t advance( token_list_item_t** stream ) { 
	if ( ! is_EOF( stream ) ) {
		(*stream) = (**stream).next;
		return TRUE;
	}
	else return FALSE;
} 
static token_t* accept( token_list_item_t** stream, token_type_t token_type ) 
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
ast_t* expression( token_list_item_t** stream ) {
	ast_t* lhs, *rhs;
	lhs = expression1( stream );
	if ( accept( stream, TOK_PLUS ) ) {
		rhs = expression( stream );
		return ast_plus( lhs, rhs );
	}
	else if ( accept( stream, TOK_MINUS ) ) {
		rhs = expression( stream );
		return ast_plus( lhs, rhs );
	}
	else return lhs;

}

ast_t* expression1( token_list_item_t** stream ) {
	ast_t* lhs, *rhs;
	lhs = atom( stream );
	if ( accept( stream, TOK_MULT ) ) {
		rhs = expression1( stream );
		return ast_mult( lhs, rhs );
	}
	else if ( accept( stream, TOK_DIVIDE ) ) {
		rhs = expression1( stream );
		return ast_div( lhs, rhs );
	}
	else return lhs;
}


ast_t* atom( token_list_item_t** stream ) {
	token_t* token;
	if( token = accept( stream, TOK_NUM )) 
		return ast_number( token->data.integer );
	else if ( token = accept( stream, TOK_IDENT ) )
		return ast_identifier( token->data.ident ) ;
	else if ( accept( stream, TOK_LPAR ) )
	{
		ast_t* expr = expression( stream ); 
		if ( accept( stream, TOK_RPAR ) )
			return expr;
		else ast_free( expr );
	}
	fprintf( stderr, "Expecting a number or an expression between parentheses"); 
	return NULL;
}



ast_t* statements( token_list_item_t** stream ){
	ast_t* lhs, *rhs;
	lhs = statement( stream );
	if ( accept( stream, TOK_SEMICOLON ) ) {
		rhs = statements( stream );
		return ast_seq( lhs, rhs );
	} 
	else return lhs;
}

ast_t* statement( token_list_item_t** stream )
{
	ast_t* result;
	
	if ( result = assignment( stream ) ) return result;
	else if ( accept( stream, TOK_LBRACE ) ) { 
		result = statements( stream );
		accept( stream, TOK_RBRACE );
		return result;
	}
	else if (result = if_then_else( stream ) ) return result;
	else if (result = while_cond( stream ) ) return result;
	else return NULL;
}


ast_t* assignment( token_list_item_t** stream ) {
	ast_t* expr;
	token_t* ident;
	if ( ident = accept( stream, TOK_IDENT )) {
		if ( accept( stream, TOK_ASSIGN ) && ( expr = expression( stream ) ) )
			return ast_assignment( ast_identifier( ident->data.ident ), expr );
		else return NULL;	
	}
	else return NULL;	
}

ast_t* if_then_else( token_list_item_t** stream ) {
	ast_t* cond = NULL;
	ast_t* yes = NULL;
	ast_t* no = NULL;

	if ( accept( stream, TOK_IF ) && 
		accept( stream, TOK_LPAR )  &&
		(cond = expression( stream )) &&
		accept( stream, TOK_RPAR ) && 
		accept( stream, TOK_THEN )  &&
		(yes = statement( stream )) && 
		accept( stream, TOK_ELSE ) &&
		(no = statement( stream )) )
		return ast_if_then_else( cond, yes, no );

	ast_free( cond );
	ast_free( yes );
	ast_free( no );
	return NULL;
}

ast_t* while_cond( token_list_item_t** stream ) {
	ast_t* cond = NULL;
	ast_t* body= NULL;
	if ( accept( stream, TOK_WHILE ) && 
		accept( stream, TOK_LPAR )  &&
		(cond = expression( stream )) &&
		accept( stream, TOK_RPAR ) && 
		(body = statement( stream )) )
		return ast_while( cond, body);

	ast_free( cond );
	ast_free( body );
	return NULL;
}


int main( int argc, char** argv )
{
	token_list_t list;
	ast_t* tree;
	token_list_item_t* stream;
	list = tokenize( "if (2+3) then x:=4+y else x:=2 ; while( 4) {y := 55 ; x := 3}" );
	token_list_foreach( &list, token_print );
	stream = list.first;
	tree = statements( &stream ); 
	ast_print( tree, stdout );

	getchar();
}


