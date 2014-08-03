#include <stdlib.h>
#include <stdio.h>
#include "common.h" 
#include "bytecode.h"

size_t bytecode_size[] = {
	1,
	1,
	1,
	1,
	1,
	1 + sizeof( lang_int_t ),
	1 + sizeof( reg_num_t ),
	1 + sizeof( reg_num_t ),
	1 + sizeof( addr_offset_t ),
	1 + sizeof( addr_offset_t ),
	1,
	1,
	1
};

/* Helpers *//*
static bytecode_t* last( bytecode_t* bc )
{
	while (bc != NULL) bc = bc->next;
	return bc;
}

static void bc_concat( bytecode_t* left, bytecode_t* right ) {
	last( left )-> next = right;
}*/

void bytecode_foreach( bytecode_t* bc, void ( action )( bytecode_t* ) ) {
	bytecode_t* next;

	if ( bc == NULL ) return;

	while( bc != NULL ) {
		next = bc->next;
		action( bc );
		bc = next;
	}
}

bytecode_t* emit_bytecode( bytecode_builder_t* builder, opcode_t opcode) {
	bytecode_t* item;
	item = (bytecode_t*) malloc( sizeof( bytecode_t ) );
	item-> opcode = opcode;
	item-> next = NULL;
	if ( builder->last == NULL ) builder->first = builder-> last = item;
	else {
		builder->last->next = item;
		builder->last = item;
	}

	return item;
}



/* Helpers to emit specific bytecodes with parameters */

static addr_offset_t* emit_delayed_jump( bytecode_builder_t* builder, opcode_t code ) {
	bytecode_t* bc;
	bc = emit_bytecode( builder, code );
	return &(bc->params.offset);
}

addr_offset_t* emit_jmp( bytecode_builder_t* builder )  { return emit_delayed_jump( builder, BC_JMP ); } 
addr_offset_t* emit_jz( bytecode_builder_t* builder )   { return emit_delayed_jump( builder, BC_JZ ); }
addr_offset_t* emit_jnz( bytecode_builder_t* builder )   { return emit_delayed_jump( builder, BC_JNZ ); }

void emit_ipushc( bytecode_builder_t* builder, lang_int_t constant ) {
	bytecode_t* bc;	
	bc = emit_bytecode( builder, BC_IPUSHC ); 
	bc->params.immediate = constant;
}

void emit_ipushreg( bytecode_builder_t* builder, reg_num_t reg) {
	bytecode_t* bc;	
	bc = emit_bytecode( builder, BC_IPUSHREG ); 
	bc->params.reg = reg;
}


void emit_ipopreg( bytecode_builder_t* builder, reg_num_t reg) {
	bytecode_t* bc;	
	bc = emit_bytecode( builder, BC_IPOPREG ); 
	bc->params.reg = reg;
}



/* For debugging purposes */

void bytecode_print( bytecode_t* bytecode ) {
	if( bytecode == NULL) { fprintf( stderr, "Trying to print NULL bytecode!"); return; }
	printf( bytecode_mnemonics[ bytecode-> opcode ] );
	switch ( bytecode->opcode )
	{
	case BC_IPUSHC:
		printf( " " );
		printf( LANG_INT_PRINTF_SPECIFICATOR, bytecode->params.immediate );
		break;

	case BC_IPUSHREG:
	case BC_IPOPREG:
		printf( " %d\n",  bytecode-> params.reg );
		break;
	case BC_JMP:
	case BC_JZ:
		printf( " %d\n",  bytecode-> params.offset );
		break;
	default:
		break;
	}

	printf("\n");
}

void bytecode_prettyprint( bytecode_t* bc ) {
	bytecode_t* next;
	size_t counter = 0;
	if ( bc == NULL ) return;


	while( bc != NULL ) {
		next = bc->next; 
		printf( "%d: ", counter );
		counter += bytecode_size[ bc-> opcode ];
		bytecode_print( bc );
		bc = next;
	}
}

