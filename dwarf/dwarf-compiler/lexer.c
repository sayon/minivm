#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "common.h"  
#include "token_list.h"



static void token_list_quick_append( token_t* token, token_list_t** first, token_list_t** last ) {
	token_list_t* item;

	item = token_list_create( token );

	if ( *first == NULL ) *first = *last = item;
	else {
		(**last).next = item;
		(*last) = item;
	} 
}

static bool_t starts_with( const char *str, const char *pattern )
{
	size_t len_pattern = strlen( pattern );
	size_t len_str = strlen( str );
	return ( len_str < len_pattern ) ? FALSE : strncmp( pattern, str, len_pattern ) == 0;
}

static char* skip_whitespaces( char* str ) {
	while( isspace( *str ) ) str++;
	return str;
}

static size_t get_number( char* str, token_t* token ) {
	size_t read = 0;
	size_t sign = 0;
	if ( (str[0] == '+' ) || (str[0] == '-') ) sign = 1;
	while( isdigit(str[ sign + read ]) ) read++;
	if ( read != 0 ) {
		token-> type  = TOK_NUM;
		token-> data.integer = atoi( str );
		return sign + read;
	}

	return 0;
}

static size_t get_keyword( char* str, token_t* token ) {
	token_type_t tok_type;
	for( tok_type = 0; tok_type < TOK_NUM; tok_type ++ )	
		if ( starts_with( str, token_strings[ tok_type ] ) ) 
		{
			token-> type = tok_type;
			return strlen( token_strings[ tok_type ] );
		}
		return 0;
}

static size_t get_ident( char* str, token_t* token ) {
	size_t size = 0;
	while( isalnum( str[ size ] ) ) size++;

	if ( size > IDENTIFIER_MAX_LENGTH ) {
		fprintf( stderr, "Identifier length exceeds allowed" ); 
		abort();
		return 0;
	};
	if ( size > 0 ) {
		token-> type = TOK_IDENT;
		strncpy( token->data.ident, str, size );
		token-> data.ident[size] = 0;
	}

	return size;
}

size_t get_token( char* str, token_t* token ) {
	int read = 0;

	read = get_keyword( str, token );
	if ( read > 0 ) return read;

	read = get_number( str, token );
	if ( read > 0 ) return read;

	read = get_ident( str, token );
	if ( read > 0 ) return read;

	return 0;
}

token_list_t* tokenize( char* const str ) {  
	char* start = str;
	token_list_t* tokens = NULL;
	token_list_t* last_token = NULL;
	if ( str == NULL ) return NULL;

	for( ;; )
	{
		token_t token;
		size_t token_size;
		start = skip_whitespaces( start );
		token_size = get_token( start, &token );
		if ( token_size == 0 ) break;
		token.start_position = start - str;
		token_list_quick_append( &token, &tokens, &last_token );
		start += token_size;
	}
	token_list_quick_append( &token_EOF, &tokens, &last_token );

	return tokens;
} 