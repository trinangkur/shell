
#ifndef __PARSER_H_
#define __PARSER_H_

typedef enum
{
  false,
  true
} Bool;

void remove_new_line(char *string);
char **get_args(char *command, Dictionary *vars);
char *get_command_string(Dictionary *aliases);

#endif