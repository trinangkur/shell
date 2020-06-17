#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

char * getPortion(char *command, int first,int last) {
  char * ref = malloc(last - first + 1);
  for (int i = first, j =0; i < last; i++, j++)
  {
    ref[j] = command[i];
  }
  ref[last] = 0;
  return ref;
}

char ** seperat(int *a, char * command, int count, int length) {
  char ** list = malloc(sizeof(char *) * (count + 1));
  int first = 0;
  for (int i = 0; i < count; i++)
  {
    list[i] = getPortion(command, first, a[i]);
    first = a[i] + 1;
  }
  list[count] = NULL;
  return list;
}

char ** split(char *command) {
  int length = strlen(command);
  int a [length];
  int count = 0;
  for (int i = 0; i < length; i++)
  {
    if (command[i] == ' ')
    {
      a[count] = i;
      count ++;
    }
  }
  a[count++] = length;
  return seperat(a, command, count, length);
}

void sighandler(int signum) {
  exit(1);
}

int main (void) {
  signal(SIGINT, SIG_IGN);
  while(1) {
    char command[255];
    printf(" my Shell$ ");
    gets(command);
    int pid = fork();
    if (pid == 0)
    {
      signal(SIGINT, sighandler);
      char **args = split(command);
      if(execvp(args[0], args) < 0) printf("Invalid Command\n");
    } else
    {
      wait(&pid);
    }
  }
  return 0;
}