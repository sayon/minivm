#ifndef TOKENS_H
#define TOKENS_H
#include <stdio.h>
#include "common.h"
#include "lang_types.h"

typedef enum {
	TOK_IF,
	TOK_THEN,
	TOK_ELSE,
	TOK_WHILE,
	TOK_PRINT,
	TOK_SKIP,
	TOK_SEMICOLON,
	TOK_ASSIGN,

	TOK_LBRACE,
	TOK_RBRACE,
	TOK_LPAR,
	TOK_RPAR,

	TOK_PLUS,
	TOK_MINUS,
	TOK_MULT,
	TOK_DIVIDE,

	TOK_NUM,
	TOK_IDENT,
	TOK_EOF,
	TOKENS_COUNT
} token_type_t;



static char* token_strings[] = {
	"if",
	"then",
	"else",
	"while",
	"print",
	"skip",
	";",
	":=",

	"{",
	"}",
	"(",
	")",

	"+",
	"-",
	"*",
	"/",

	"<num>",
	"<ident>",
	"<EOF>"
};

typedef struct  {
	token_type_t type;
	union {
		lang_int_t integer;
		char ident[IDENTIFIER_MAX_LENGTH];
	} data;
} token_t;

extern token_t token_EOF;

typedef struct token_list_item_t {
	token_t token;
	struct token_list_item_t* next;
} token_list_item_t;

typedef struct {
	struct token_list_item_t* first;
	struct token_list_item_t* last;
} token_list_t;


token_list_item_t* token_list_item_create( token_t token );

void token_list_add( token_list_t* list, token_t token );

token_list_t token_list_create( void );

void token_list_foreach( token_list_t* list, void (action)(token_t*) );

void token_print( token_t* token ) ;

#endif