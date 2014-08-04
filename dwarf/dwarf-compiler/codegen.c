
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bytecode.h"
#include "codegen.h"

typedef struct {
	char* registers[REGISTERS_COUNT];
	size_t occupied_regs;
} generator_context_t;

static generator_context_t* generator_context_create( void ) {
	generator_context_t* ctx;
	ctx = (generator_context_t*) malloc( sizeof( generator_context_t ) );
	memset( ctx->registers, 0, REGISTERS_COUNT );
	ctx-> occupied_regs = 0;
	return ctx;
}

static void generator_context_destroy( generator_context_t* ctx ) {	
	size_t i;
	for( i = 0; i < ctx->occupied_regs; i++ ) free( ctx->registers[ i ] );
	free( ctx ); 
}

static size_t register_idx( generator_context_t* ctx, const char* name ) {
	size_t i;
	for( i = 0; i < ctx-> occupied_regs; i++ ) 
		if( strcmp( name, ctx-> registers[ i ] ) == 0 ) return i;

	if ( ctx-> occupied_regs == REGISTERS_COUNT ) { fprintf( stderr, "Too many variables, this register allocator can not allocate more registers"); abort(); }

	ctx-> registers[ i ] = (char*) malloc( strlen( name ) + 1 );
	strcpy( ctx-> registers[ i ], name );
	ctx-> occupied_regs ++;
	return i;
}

static size_t generate( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx );

static size_t generate_number( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	emit_ipushc( bc, node->attributes.as_number );
	return bytecode_size[ BC_IPUSHC ];
}

static size_t generate_plus( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( node->attributes.as_binop.left, bc, ctx );
	second = generate( node->attributes.as_binop.right, bc, ctx );
	emit_bytecode( bc, BC_IADD );
	return first + bytecode_size[ BC_IADD ] + second;
}

static size_t generate_minus( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( node->attributes.as_binop.left, bc, ctx );
	second = generate( node->attributes.as_binop.right, bc, ctx );
	emit_bytecode( bc, BC_ISUB );
	return first + bytecode_size[ BC_ISUB ] + second ;
}

static size_t generate_mult( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( node->attributes.as_binop.left, bc, ctx );
	second = generate( node->attributes.as_binop.right, bc, ctx );
	emit_bytecode( bc, BC_IMUL );
	return first + bytecode_size[ BC_IMUL ] + second;
}

static size_t generate_div( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( node->attributes.as_binop.left, bc, ctx );
	second = generate( node->attributes.as_binop.right, bc, ctx );
	emit_bytecode( bc, BC_IDIV );
	return first + bytecode_size[ BC_IDIV ] + second;
}

static size_t generate_gt_helper( ast_t* left, ast_t* right, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( left, bc, ctx );
	second = generate( right, bc, ctx );
	emit_bytecode( bc, BC_ICMP );
    emit_ipushc( bc, 1 );
	emit_bytecode( bc, BC_EQ );
	return first + second 
		+ bytecode_size[ BC_ICMP ] 
		+ bytecode_size[ BC_IPUSHC ] 
		+ bytecode_size[ BC_EQ ];
}

static size_t generate_gt( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	return generate_gt_helper( node->attributes.as_binop.left, node->attributes.as_binop.right, bc, ctx );
}

static size_t generate_lt( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) { //a < b === b > a
	return generate_gt_helper( node->attributes.as_binop.right, node->attributes.as_binop.left, bc, ctx );
}


static size_t generate_eq( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t parts;
	parts = generate( node->attributes.as_binop.left, bc, ctx ) + generate( node->attributes.as_binop.right, bc, ctx );
	emit_bytecode( bc, BC_EQ );
	return parts + bytecode_size[ BC_EQ ];
}

static size_t generate_seq( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t first, second;
	first = generate( node->attributes.as_seq.first, bc, ctx );
	second = generate( node->attributes.as_seq.second, bc, ctx );
	return first + second;
}

static size_t generate_assign( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t rhs;
	rhs = generate( node->attributes.as_assign.value, bc, ctx );
	emit_ipopreg( bc, register_idx( ctx, node ->attributes.as_assign.identifier->attributes.as_identifier.name ) );
	return rhs + bytecode_size[ BC_IPOPREG ];
}

static size_t generate_identifier( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	emit_ipushreg( bc, register_idx( ctx, node-> attributes.as_identifier.name ) );
	return bytecode_size[ BC_IPUSHREG ];
}

static size_t generate_print( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t expr_size;
	expr_size = generate( node->attributes.as_unop.expr, bc, ctx );
	emit_bytecode( bc, BC_PRINT );
	return expr_size + bytecode_size[ BC_PRINT ];
}

static size_t generate_if( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	size_t cond_size, yes_size, no_size;
	addr_offset_t* no;
	addr_offset_t* end;

	cond_size = generate( node-> attributes.as_if.cond, bc, ctx );
	no = emit_jz( bc );
	yes_size = generate( node-> attributes.as_if.yes, bc, ctx );
	end = emit_jmp( bc );
	no_size = generate( node-> attributes.as_if.no, bc, ctx );

	*no = yes_size;
	*end = no_size + bytecode_size[ BC_JMP ];
	return 
		cond_size 
		+ bytecode_size[ BC_JZ ] 
	+ yes_size 
		+ bytecode_size[ BC_JMP ]
	+ no_size;
}

static size_t generate_while( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	//if (cond == 0) jmp to after the loop, in the loop beginning jump to this adress.
	size_t cond_size, body_size;
	addr_offset_t* loop_start;
	addr_offset_t* loop_end;

	cond_size = generate( node-> attributes.as_while.cond, bc, ctx );
	loop_end = emit_jz( bc );
	body_size = generate( node-> attributes.as_while.body, bc, ctx );
	loop_start = emit_jmp( bc );
	*loop_start = - ((addr_offset_t)( bytecode_size[ BC_JZ ] + body_size ));
	*loop_end = body_size;

	return cond_size 
		+ bytecode_size[ BC_JZ ]
	+ body_size
		+ bytecode_size[ BC_JMP ];
}

static size_t generate( ast_t* node, bytecode_builder_t* bc, generator_context_t* ctx ) {
	switch( node->type ) {
	case AST_IF:		 return generate_if( node, bc, ctx ); 
	case AST_WHILE:		 return generate_while( node, bc, ctx );
	case AST_SKIP:		 return 0;
	case AST_ASSIGN:	 return generate_assign( node, bc, ctx ); 
	case AST_NUMBER:	 return generate_number( node, bc, ctx ); 
	case AST_PLUS:		 return generate_plus( node, bc, ctx ); 
	case AST_MINUS:		 return generate_minus( node, bc, ctx );
	case AST_MULT:		 return generate_mult( node, bc, ctx );
	case AST_DIVIDE:	 return generate_div( node, bc, ctx ); 
	case AST_LT:	     return generate_lt( node, bc, ctx ); 
	case AST_EQ:         return generate_eq( node, bc, ctx ); 
	case AST_GT:         return generate_gt( node, bc, ctx );
	case AST_SEQ:		 return generate_seq( node, bc, ctx ); 
	case AST_IDENTIFIER: return generate_identifier( node, bc, ctx ); 
	case AST_PRINT:		 return generate_print( node, bc, ctx ); 

	default:
		fprintf( stderr, "Can't generate code for such node type! %d", node->type );
		abort() ;
	}
}

bytecode_builder_t generate_from_ast( ast_t* root ) {
	bytecode_builder_t bc;
	generator_context_t* ctx;  
	bc.first = bc.last = NULL;
	ctx = generator_context_create();

	generate( root, &bc, ctx );
	emit_bytecode( &bc, BC_HALT );
	generator_context_destroy( ctx );
	return bc;
}
