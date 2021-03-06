#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dictionary.h"
#include "src.h"
#include "util.h"

int find_char(char *string, char key) {
  for (int i = 0; string[i] != '\0'; i++) {
    if (string[i] == key) {
      return 1;
    }
  }
  return 0;
}

int is_valid_key(char *key)
{
  return !(strstr(key, "\n") || strstr(key, " ") || strstr(key, "\t") || strstr(key, "\"") || strstr(key, "'"));
}

void handle_alias(Dictionary *aliases, char **args, int *code)
{
  if (args[1] == NULL)
    show(aliases);
  else {
    char **key_value = splitIntoTwo(args[1], '=');
    if (key_value == NULL) {
      printf("%s: '%s'\n", args[1], get_value(aliases, args[1]));
    }else {
      if (!is_valid_key(key_value[0])) {
        printf("alias: Invalid key. ' ', '\\t', '\\n', ''' and '\"' are not allowed.\n");
        *code = 1;
        return;
      }
      add(aliases, key_value[0], key_value[1]);
    }
  }
}

void handle_cd(char *path, int *code)
{
  if (chdir(path == NULL ? getenv("HOME") : path) < 0)
  {
    printf("'cd' command couldn't be executed. Please check the path or permission!!\n");
    *code = 1;
  }
}

void handle_var(Dictionary *vars, char** args, int *code) {
  if (args[1] != NULL) {
    printf("invalid format\n");
    *code = 1;
    return;
  }
  char ** key_value = splitIntoTwo(args[0], '=');
  if (!is_valid_key(key_value[0])) {
    printf("Invalid key. ' ', '\\t', '\\n', ''' and '\"' are not allowed.\n");
    *code = 1;
    return;
  }
  add(vars, key_value[0], key_value[1]);
}

int handle_internal_commands(char **args, Dictionary *aliases,Dictionary *vars, int *code)
{
  if (strcmp(args[0], "cd") == 0)
    handle_cd(args[1], code);
  else if (strcmp(args[0], "alias") == 0)
    handle_alias(aliases, args, code);
  else if (strcmp(args[0], "unalias") == 0)
    remove_key(aliases, args[1]);
  else if (find_char(args[0],'='))
    handle_var(vars,args,code);
  else
    return 0;

  return 1;
}