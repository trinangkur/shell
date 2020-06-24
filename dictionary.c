#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

Dictionary *create_dictionary() {
  Dictionary *dictionary = malloc(sizeof(Dictionary));
  dictionary->start = NULL;
  return dictionary;
}

Pair *create_pair(char *key, char *value) {
  Pair *new_pair = malloc(sizeof(Pair));
  new_pair->key = malloc(sizeof(char) * (strlen(key) + 1));
  new_pair->value = malloc(sizeof(char) * (strlen(value) + 1));
  strcpy(new_pair->key, key);
  strcpy(new_pair->value, value);
  new_pair->next = NULL;
  return new_pair;
}

void add(Dictionary *dictionary, char *key, char *value) {
  Pair *current_pair = dictionary->start;

  while (current_pair != NULL) {
    if (strcmp(current_pair->key, key) == 0) {
      current_pair->value = realloc(current_pair->value, sizeof(char) * (strlen(value) + 1));
      strcpy(current_pair->value, value);
      return;
    }
    current_pair = current_pair->next;
  }

  Pair *new_pair = create_pair(key, value);
  new_pair->next = dictionary->start;
  dictionary->start = new_pair;
}

char *get_value(Dictionary *dictionary, char *key) {
  Pair *current_pair = dictionary->start;

  while (current_pair != NULL)
  {
    if (strcmp(current_pair->key, key) == 0)
    {
      char *value = malloc(sizeof(char) * (strlen(current_pair->value) + 1));
      strcpy(value, current_pair->value);
      return value;
    }
    current_pair = current_pair->next;
  }

  return NULL;
}

void show(Dictionary *dictionary)
{
  Pair *current_pair = dictionary->start;

  while (current_pair != NULL)
  {
    printf("%s: '%s'\n", current_pair->key, current_pair->value);
    current_pair = current_pair->next;
  }
}

void remove_key(Dictionary *dictionary, char *key)
{
  Pair **current_node_link = &(dictionary->start);

  while ((*current_node_link) != NULL)
  {
    if (strcmp((*current_node_link)->key, key) == 0)
    {
      Pair *pair_to_delete = (*current_node_link);
      *current_node_link = (*current_node_link)->next;
      if (dictionary->start == pair_to_delete)
        dictionary->start = pair_to_delete->next;
      free(pair_to_delete->key);
      free(pair_to_delete->value);
      return free(pair_to_delete);
    }
    current_node_link = &(*current_node_link)->next;
  }
}
