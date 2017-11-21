#ifndef LIB_FUNC_H
#define LIB_FUNC_H

enum action_type
{
    FIND_MAX = 0,
	FIND_MIN,
};

int find_value(int mass[], int count, int action);

#endif //LIB_FUNC_H