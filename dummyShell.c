#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#include "util.h"
#include "dictionary.h"
#include "parser.h"
#include "src.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define CYAN "\x1b[36m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

void handle_cmd_not_found(char *command){
  printf(RED "tsh: " RESET);
  printf("command not found: %s\n", command);
  exit(127);
}

void prompt(int exit_code) {
  char cwd[200];
  printf("tsh ");
  
  getcwd(cwd, sizeof(cwd));
  printf(CYAN "%s ", cwd);

  if (exit_code != 0) {
    printf(RED "$ ");
  }
  else {
    printf(GREEN "$ ");
  }

  printf(RESET);
}

void sighandler(int signum) {
  exit(1);
}

void handle_command (char **args, Dictionary *aliases, Dictionary *vars, int *exit_code) {
  if (handle_internal_commands(args, aliases, vars, exit_code)) {
      return;
    }
    *exit_code = 0;
    int pid = fork();
    if (pid == 0) {
      signal(SIGINT, sighandler);
      execvp(args[0], args);
      handle_cmd_not_found(args[0]);
      *exit_code = 1;
      exit(0);
    } else
    {
      wait(&pid);
    }
}

  int main (void) {
  char *command;
  int exit_code;
  Dictionary *aliases = create_dictionary();
  Dictionary *vars = create_dictionary();

  signal(SIGINT, SIG_IGN);
  while(1) {
    prompt(exit_code);
    command = get_command_string(aliases);
    char **args = get_args(command, vars);
    handle_command(args, aliases, vars, &exit_code);
  }
  return 0;
}