#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lib-func.h"

int find_value(int mass[], int size, int action)
{
  int i = 0;
  int max = mass[0];
  int min = mass[0];
  for (i=0;i<size;i++)
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

int* sort(int mass[], int size, int action) {

  if(action == 2) {
    int lim_n;
    int lim_ch;

    if (size % 2 == 0) {
      lim_n = size - 1;
      lim_ch = size;
    }
    else {
      lim_n = size;
      lim_ch = size - 1;
    }

    int count;

    do {
      count = 0;
      for (int i = 1; i < lim_n; i = i + 2) {
        if (mass[i] > mass[i + 1]) {
          int temp = mass[i];
          mass[i] = mass[i + 1];
          mass[i + 1] = temp;
          count++;
        }
      }

      for (int i = 0; i < lim_ch; i = i + 2) {
        if (mass[i] > mass[i + 1]) {
          int temp = mass[i];
          mass[i] = mass[i + 1];
          mass[i + 1] = temp;
          count++;
        }
      }
    } while (count != 0);

    return  mass;
  }
}
