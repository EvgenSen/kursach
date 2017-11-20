#include <stdlib.h>
#include <stdio.h>

#define FILE_NAME "input.txt"

int
main(int argc, char *argv[])
{
  int i, min, max, value;
  unsigned int count;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s <count> <min> <max>\n", argv[0]);
    return 1;
  }

  count = atoi(argv[1]);
  min = atoi(argv[2]);
  max = atoi(argv[3]);

  srand(time(NULL));

  FILE * fp = fopen(FILE_NAME, "w");
  if (fp == NULL)
  {
    fprintf(stderr, "Error: Can not open file \n");
    return 1;
  }

  for (i = 0; i < count; i++) {
    value = min + rand() % (max - min);
    fprintf(fp, "%d\n", value);
  }

  fclose (fp);
  return 0;
}
