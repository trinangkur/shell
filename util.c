#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include"util.h"


char *copy_string(char *str, int start, int end)
{
  int i;
  char *n_str = calloc(end - start +1, sizeof(char));

  for (i = start; i < end; i++)
  {
    n_str[i - start] = str[i];
  }

  n_str[i] = '\0';
  return n_str;
}

char **splitIntoTwo(char *exp, char key)
{
  char **split = malloc(sizeof(char *) * 2);
  int ins_len = strlen(exp);

  for (int i = 0; i < ins_len; i++)
  {
    if (exp[i] == key)
    {
      split[0] = copy_string(exp, 0, i);
      split[1] = copy_string(exp, i + 1, ins_len);

      return split;
    }
  }

  return NULL;
}
