
#include <ctype.h> /* Using function tolower. */
#include "svimrdb.h"

char * strlowercase(char * str)
{
	char * s = str;
	while (*str++)
		*str = tolower(*str);
	return s;
}
