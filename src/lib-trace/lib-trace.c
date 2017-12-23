#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "lib-trace.h"

void trace_msg(int type, char *format, ...)
{
    va_list ap;
    int length, size;
    char str[1024] = {0};
    char prefix_time[80] = {0};
    char* format_time = "%d.%m.%Y %H:%M:%S"; // 23.12.2017 22:02:21
//  char* format_time = "%H:%M:%S";          //            22:02:21
    time_t cur_time = time(NULL);
    struct tm * timeinfo = localtime(&cur_time);

    if ( type == ERR_MSG && TRACE_ERR == 0)
        return;
    if ( type == DBG_MSG && TRACE_DBG == 0)
        return;
    if ( type == INF_MSG && TRACE_INF == 0)
        return;

    strftime(prefix_time, sizeof(prefix_time), format_time, timeinfo);

    length = strlen(str);
    size = sizeof(str) - length - 10;

    va_start(ap, format);
    length = vsnprintf(&str[length], size, format, ap);
    va_end(ap);

    size = strlen(str);
    while (size && (str[size - 1] == '\n'))
        str[--size] = '\0';

    fprintf(stderr, "[%s]%s%s\n", prefix_time, 
                                  type == ERR_MSG ? "\033[1;31m[ERR]\033[0m" :          // Красный
                                  type == DBG_MSG ? "\033[1;33m[DBG]\033[0m" :          // Жельый
                                  type == INF_MSG ? "\033[1;32m[INF]\033[0m" :          // Зеленый
                                                    "\033[1;31m[UNK]\033[0m" , str);
}
