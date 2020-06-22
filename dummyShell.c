#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

typedef enum {
  false,
  true
} Bool;

void replace_new_line(char *string) {
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
    replace_new_line(new_line);
    strcat(total_command, "\n");
    strcat(total_command, new_line);
    args = parse_command_string(total_command);
  }
  return args;
}

void sighandler(int signum) {
  exit(1);
}

int main (void) {
  char command[255];
  int exit_code;
  char cwd[100];
  char *home = getenv("HOME");
  signal(SIGINT, SIG_IGN);
  while(1) {
    getcwd(cwd, sizeof(cwd));
    printf("\ntcs$ \033[0m(\033[0;36m%s\033[0m)-> ",  cwd);
    fgets(command, 100, stdin);
    replace_new_line(command);
    int pid = fork();
    if (pid == 0)
    {
      signal(SIGINT, sighandler);
      char **args = get_args(command);
      if (strcmp(args[0], "cd") == 0)
      {
        exit_code = chdir(args[1] == NULL ? home : args[1]);
        if (exit_code < 0)
          printf("\033[0;31m 'cd' cannot execute\n");
      }
      else
      {
        exit_code = execvp(args[0], args);
        if (exit_code == -1)
          printf("\033[0;31mcommand not found!!\n");
      }
    } else
    {
      wait(&pid);
    }
  }
  return 0;
}