#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALIASES 100
#define MAX_ALIAS_NAME 100
#define MAX_ALIAS_VALUE 100

struct alias {
  char name[MAX_ALIAS_NAME];
  char value[MAX_ALIAS_VALUE];
};

struct alias aliases[MAX_ALIASES];
int num_aliases = 0;

void print_alias(struct alias a) {
  printf("%s='%s'\n", a.name, a.value);
}

void print_aliases() {
  for (int i = 0; i < num_aliases; i++) {
    print_alias(aliases[i]);
  }
}

int find_alias_index(char* name) {
  for (int i = 0; i < num_aliases; i++) {
    if (strcmp(aliases[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

void print_alias_names(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    int index = find_alias_index(argv[i]);
    if (index != -1) {
      print_alias(aliases[index]);
    }
  }
}

void set_alias(char* name, char* value) {
  int index = find_alias_index(name);
  if (index == -1) {
    if (num_aliases >= MAX_ALIASES) {
      printf("Too many aliases\n");
      return;
    }
    index = num_aliases++;
    strcpy(aliases[index].name, name);
  }
  strcpy(aliases[index].value, value);
}

void process_args(int argc, char** argv) {
  if (argc == 1) {
    print_aliases();
  } else if (argc == 2) {
    print_alias_names(argc, argv);
  } else {
    for (int i = 1; i < argc; i++) {
      char* arg = argv[i];
      char* equals = strchr(arg, '=');
      if (equals == NULL) {
        print_alias_names(2, &arg);
      } else {
        *equals = '\0';
        set_alias(arg, equals+1);
      }
    }
  }
}

int main(int argc, char** argv) {
  process_args(argc, argv);
  return 0;
}
