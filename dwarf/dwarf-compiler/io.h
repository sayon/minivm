#ifndef FILE_IO
#define FILE_IO

#include <stdio.h>
#include "bytecode.h"

char* read_file( FILE* file );
void write_bytecode( bytecode_t* bytecode );


#endif