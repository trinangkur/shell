#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include "parser.h"

void remove_new_line(char *string) {
  for (unsigned index = 0; string[index] != '\0'; index++) {
    if (string[index] == '\n')
      string[index] = '\0';
  }
}

char **parse_command_string(char *command) {
  char **args = malloc(sizeof(char *) * strlen(command));
  unsigned arg_index = 0, string_index = 0;
  Bool has_escaped = false, is_double_quote = false, is_single_quote = false;
  args[0] = malloc(sizeof(char) * strlen(command));

  for (unsigned index = 0; command[index] != '\0'; index++) {
    if (has_escaped) {
      args[arg_index][string_index++] = command[index];
      has_escaped = false;
      continue;
    }

    if (command[index] == '\'' && !is_double_quote) {
      is_single_quote = !is_single_quote;
      continue;
    }

    if (command[index] == '"' && !is_single_quote) {
      is_double_quote = !is_double_quote;
      continue;
    }

    if (command[index] == '\\') {
      has_escaped = true;
      continue;
    }

    if (command[index] == ' ' && !is_double_quote && !is_single_quote) {
      args[arg_index][string_index++] = '\0';
      args[arg_index] = realloc(args[arg_index], sizeof(char) * string_index);
      args[++arg_index] = malloc(sizeof(char) * strlen(command));
      string_index = 0;
      continue;
    }

    args[arg_index][string_index++] = command[index];
  }

  if (is_double_quote || is_single_quote) {
    return NULL;
  }
  args[arg_index][string_index] = '\0';
  args = realloc(args, sizeof(char *) * (arg_index + 2));
  args[arg_index + 1] = NULL;
  return args;
}

char **get_args(char *command)
{
  char total_command[1000] = "\0";
  strcpy(total_command, command);
  char **args = parse_command_string(command);

  while (args == NULL)
  {
    printf("dquote> ");
    char new_line[100];
    fgets(new_line, 100, stdin);
    remove_new_line(new_line);
    strcat(total_command, "\n");
    strcat(total_command, new_line);
    args = parse_command_string(total_command);
  }
  return args;
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