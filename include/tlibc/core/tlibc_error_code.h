#ifndef _H_TLIBC_ERROR_CODE
#define _H_TLIBC_ERROR_CODE

#include "tlibc/platform/tlibc_platform.h"

typedef enum _TLIBC_ERROR_CODE
{
    E_TLIBC_NOERROR = 0,
    E_TLIBC_ERROR = -1,
	E_TLIBC_IGNORE = -2,
    E_TLIBC_OUT_OF_MEMORY = -3,
	E_TLIBC_CAN_NOT_OPEN_FILE = -4,
	E_TLIBC_XML_SYNTAX_ERROR = -5,
	E_TLIBC_XML_TAG_MISMATCH = -6,
	E_TLIBC_CONVERT_ERROR = -7,
	E_TLIBC_AGAIN = -8,
	E_TLIBC_BADZIPFILE = -9,
	E_TLIBC_END_OF_LIST_OF_FILE = -10,
}TLIBC_ERROR_CODE;

#define TLIBC_ERROR_CODE_NUM 15

#endif
