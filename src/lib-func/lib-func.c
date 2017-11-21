#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lib-func.h"

int find_value(int mass[], int count, int action)
{
  int i = 0;
  int max = mass[0];
  int min = mass[0];
  for (i=0;i<count;i++)
  {
    if (max < mass[i])
      max = mass[i];
    if (min > mass[i])
      min = mass[i];
  }
  if (action == FIND_MAX)
    return max;
  else
    return min;
}
