#ifndef PARSER_H
#define PARSER_H

#include "token_list.h"
#include "ast.h"

ast_t* parse( token_list_t* list, const char* const code );

#endif