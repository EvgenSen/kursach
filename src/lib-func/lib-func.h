#ifndef LIB_FUNC_H
#define LIB_FUNC_H

enum action_type
{
    FIND_MAX,
	FIND_MIN,
	SORT
};

int find_value(int *mass, int size, int action);
int* sort(int *mass, int size, int action);

#endif //LIB_FUNC_H