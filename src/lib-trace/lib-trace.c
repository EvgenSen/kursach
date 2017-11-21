#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "lib-trace.h"

void trace_msg(int type, char *format, ...)
{
    va_list ap;
    int length, size;
    char str[1024] = {0};

    if ( type == ERR_MSG && TRACE_ERR == 0)
    	return;
    if ( type == DBG_MSG && TRACE_DBG == 0)
    	return;

    length = strlen(str);
    size = sizeof(str) - length - 10;

    va_start(ap, format);
    length = vsnprintf(&str[length], size, format, ap);
    va_end(ap);

    size = strlen(str);
    while (size && (str[size - 1] == '\n'))
        str[--size] = '\0';

    fprintf(stderr, "%s%s\n", type == ERR_MSG ? "[ERR]" : "[DBG]", str);
}
