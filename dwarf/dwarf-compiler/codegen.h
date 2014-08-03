#ifndef CODEGEN_H
#define CODEGEN_H
#include "AST.h"
#include "bytecode.h"

bytecode_builder_t generate_from_ast( ast_t* root );


#endif