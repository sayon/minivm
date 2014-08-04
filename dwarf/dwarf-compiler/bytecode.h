#ifndef BYTECODE_H
#define BYTECODE_H
 
#include <stdlib.h>
#include "common.h"

typedef enum {
	BC_HALT,
	BC_IADD,
	BC_ISUB,
	BC_IMUL,
	BC_IDIV,
	BC_DUP,
	BC_IPUSHC,
	BC_IPUSHREG,
	BC_IPOPREG,
	BC_JMP,
	BC_JZ,
	BC_JNZ,
	BC_ICMP,
	BC_EQ,
	BC_PRINT,
	BC_DEBUG
} opcode_t;

static char* bytecode_mnemonics[] = {
	"HALT",
	"IADD",
	"ISUB",
	"IMUL",
	"IDIV",
	"DUP",
	"IPUSHC",
	"IPUSHREG",
	"IPOPREG",
	"JMP",
	"JZ",
	"JNZ",
	"ICMP",
	"EQ",
	"PRINT",
	"DEBUG"
};

typedef signed long addr_offset_t;
typedef short reg_num_t;

extern size_t bytecode_size[];
 

typedef struct bytecode_t {
	opcode_t opcode;
	union { 
		reg_num_t reg;
		addr_offset_t offset;
		lang_int_t immediate;
	} params;
	struct bytecode_t* next;
} bytecode_t;

typedef struct {
	bytecode_t* first;
	bytecode_t* last;
}
bytecode_builder_t;
  
 

bytecode_t* emit_bytecode( bytecode_builder_t* builder, opcode_t opcode ); 


/* Helpers to emit specific bytecodes with parameters */
 

addr_offset_t* emit_jmp( bytecode_builder_t* builder ); 
addr_offset_t* emit_jz( bytecode_builder_t* builder );
addr_offset_t* emit_jnz( bytecode_builder_t* builder );


void emit_ipushc( bytecode_builder_t* builder, lang_int_t constant );
void emit_ipushreg( bytecode_builder_t* builder, reg_num_t reg);
void emit_ipopreg( bytecode_builder_t* builder, reg_num_t reg);




/* Helpers */
void bytecode_foreach( bytecode_t* bc, void ( action )( bytecode_t* ) );
void bytecode_print( bytecode_t* bytecode );
void bytecode_prettyprint( bytecode_t* bc );
void bytecode_free( bytecode_t* bc );
void bytecode_flush( bytecode_t* bc, FILE* file);
#endif