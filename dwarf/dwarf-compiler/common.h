#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>


#define IDENTIFIER_MAX_LENGTH 64


typedef int64_t lang_int_t;
#define LANG_INT_PRINTF_SPECIFICATOR "%ld"

#define REGISTERS_COUNT 64


typedef enum {
	FALSE = 0,
	TRUE = 1
} bool_t;




#endif