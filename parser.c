#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "parser.h"

char **remove_unnecessary_arg(char **args) {
  char **new_args = malloc(sizeof(char *) * 100);
  unsigned new_arg_index = 0, index;
  for (index = 0; args[index] != NULL; index++)
  {
    if (strcmp(args[index], "") != 0)
      new_args[new_arg_index++] = args[index];
    else
      free(args[index]);
  }
  new_args[index] = NULL;
  free(args);
  return new_args;
}

void remove_new_line(char *string) {
  for (unsigned index = 0; string[index] != '\0'; index++) {
    if (string[index] == '\n')
      string[index] = '\0';
  }
}

char *get_key(char *str) {
  char *key = malloc(sizeof(char) * 30);
  unsigned index;
  for (index = 0; str[index] != ']' && str[index] != '\0'; index++)
  {
    key[index] = str[index];
  }
  key[index] = '\0';
  return key;
}

char **parse_command_string(char *command_string, Dictionary *vars) {
  char **args = malloc(sizeof(char *) * strlen(command_string));
  unsigned arg_num = 0, string_pos = 0;
  Bool is_escaped = false, double_quote = false, single_quote = false;
  args[0] = malloc(sizeof(char) * strlen(command_string));

  for (unsigned index = 0; command_string[index] != '\0'; index++) {
    if (is_escaped) {
      args[arg_num][string_pos++] = command_string[index];
      is_escaped = false;
      continue;
    }

    if (command_string[index] == '\\') {
      is_escaped = true;
      continue;
    }

    if (command_string[index] == '"' && !single_quote) {
      double_quote = !double_quote;
      continue;
    }

    if (command_string[index] == '\'' && !double_quote) {
      single_quote = !single_quote;
      continue;
    }

    if (command_string[index] == ' ' && !double_quote && !single_quote) {
      args[arg_num][string_pos] = '\0';
      args[arg_num] = realloc(args[arg_num], sizeof(char) * (string_pos + 1));
      args[++arg_num] = malloc(sizeof(char) * strlen(command_string));
      string_pos = 0;
      continue;
    }

    if (command_string[index] == '[' && !double_quote && !single_quote) {
      char *key = get_key(command_string + index + 1);
      char *value = get_value(vars, key);
      value == NULL && (value = "");
      for (unsigned i = 0; value[i] != '\0'; i++) {
        args[arg_num][string_pos++] = value[i];
      }
      index += strlen(key) + 1;
      command_string[index] == '\0' && (index--);
      continue;
    }

    args[arg_num][string_pos++] = command_string[index];
  }

  args[arg_num][string_pos] = '\0';
  args[arg_num] = realloc(args[arg_num], sizeof(char) * (string_pos + 1));

  if (double_quote || single_quote) {
    return NULL;
  }
  args = realloc(args, sizeof(char *) * (arg_num + 2));
  args[arg_num + 1] = NULL;

  return args;
}

char **get_args(char *command, Dictionary *vars)
{
  char total_command[1000] = "\0";
  strcpy(total_command, command);
  char **args = parse_command_string(command, vars);

  while (args == NULL)
  {
    printf("dquote> ");
    char new_line[100];
    fgets(new_line, 100, stdin);
    remove_new_line(new_line);
    strcat(total_command, "\n");
    strcat(total_command, new_line);
    args = parse_command_string(total_command, vars);
  }
  return remove_unnecessary_arg(args);
}

char *get_first_token(char *command_string)
{
  char *token = malloc(sizeof(char) * 30);
  unsigned index;
  for (index = 0; command_string[index] != ' ' && command_string[index] != '\0'; index++)
    token[index] = command_string[index];
  token[index] = '\0';
  return realloc(token, sizeof(char) * (strlen(token) + 1));
}

char *prepend_alias_value(char *command_string, char *alias_value, unsigned first_token_length)
{
  char *total_command = malloc(sizeof(char) * 100);
  strcpy(total_command, alias_value);
  strcat(total_command, command_string + first_token_length);
  free(command_string);
  return realloc(total_command, sizeof(char) * (strlen(total_command) + 1));
}

char *get_command_string(Dictionary *aliases)
{
  char *command_string = malloc(sizeof(char) * 100);
  fgets(command_string, 100, stdin);
  remove_new_line(command_string);
  command_string = realloc(command_string, sizeof(char) * (strlen(command_string) + 1));
  char *first_token = get_first_token(command_string);
  char *alias_value = get_value(aliases, first_token);
  free(first_token);
  if (alias_value != NULL)
  {
    command_string = prepend_alias_value(command_string, alias_value, strlen(first_token));
  }
  return command_string;
}