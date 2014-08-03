#include <string.h>
#include "io.h"

#define MAX_FILE_SIZE 65535
char* read_file( FILE* file ) {
	char* str ;
	size_t read_size;
	str = (char*) malloc( MAX_FILE_SIZE + 1 );
	read_size = fread( str, 1, MAX_FILE_SIZE, file );
	str[ read_size ] = 0;
	return str;

	/*size_t buf_size = 64;
	size_t read_size = 0;
	char *str;
	str = (char*) malloc( buf_size );
	read_size += fread( str, 1, buf_size, file );
	while( !feof( file )) {
	buf_size *=2;
	str = (char*) realloc(str, buf_size );
	read_size += fread( str + buf_size /2 - 1 , 1, buf_size /2 + 1, file );
	}
	str[ read_size ] = 0;
	return str;*/
}

void write_bytecode( bytecode_t* bytecode );