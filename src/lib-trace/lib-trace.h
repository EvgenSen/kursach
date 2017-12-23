#ifndef LIB_TRACE_H
#define LIB_TRACE_H

#define TRACE_ERR 1
#define TRACE_DBG 1
#define TRACE_INF 1

enum trace_msg_type
{
    ERR_MSG,
	DBG_MSG,
	INF_MSG,
};

void trace_msg(int type, char *format, ...);

#endif //LIB_TRACE_H