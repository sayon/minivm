#ifndef TOKENS_H
#define TOKENS_H
#include <stdio.h>
#include "common.h" 

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
	size_t start_position;
	union {
		lang_int_t integer;
		char ident[IDENTIFIER_MAX_LENGTH];
	} data;
} token_t;

extern token_t token_EOF;

void token_print( token_t* token ); 

#endif