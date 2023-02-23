#include "main.h"



/**
* struct alias
* @name:
* @value:
*/
struct alias 
{
    char name[MAX_ALIAS_NAME];
    char value[MAX_ALIAS_VALUE];
};

struct alias aliases[MAX_ALIASES];
int num_aliases = 0;

/**
* print_alias - Prints the alias
* @a: alias
*/
void print_alias(struct alias a) 
{
    printf("%s='%s'\n", a.name, a.value);
}



/**
* print_aliases
*/
void print_aliases() 
{
    int i;
    
    for (i = 0; i < num_aliases; i++) 
    {
        print_alias(aliases[i]);
    }
}



/**
* find_alias_index
* @name:
*/
int find_alias_index(char* name) 
{
    int i;

    for (i = 0; i < num_aliases; i++)
    {
        if (strcmp(aliases[i].name, name) == 0) 
        {
            return i;
        }
    }
    return -1;
}



/**
* print_alias_names
* @argc:
* @argv:
*/
void print_alias_names(int argc, char** argv) 
{
    int i;
    
    for (i = 1; i < argc; i++) 
    {
        int index = find_alias_index(argv[i]);

        if (index != -1) 
        {
            print_alias(aliases[index]);
        }
    }
}



/**
* set_alias - Set the alias
* @name:
* @value:
*/
void set_alias(char* name, char* value) 
{
    int index = find_alias_index(name);

    if (index == -1) 
    {
        if (num_aliases >= MAX_ALIASES)
        {

            printf("Too many aliases\n");
             return;
         }
        index = num_aliases++;
        strcpy(aliases[index].name, name);
    }
    strcpy(aliases[index].value, value);
}



/**
* process_args
* @argc: 
* @argv: 
*/
void process_args(int argc, char** argv) 
{
    if (argc == 1) 
    {
        print_aliases();
    } 
    else if (argc == 2) 
    {
        print_alias_names(argc, argv);
    } 
    else 
    {
        int i;

        for (i = 1; i < argc; i++) 
        {
            char* arg = argv[i];
            char* equals = strchr(arg, '=');

            if (equals == NULL) 
            {
                print_alias_names(2, &arg);
            } 
            else 
            {
                *equals = '\0';
                set_alias(arg, equals+1);
            }
        }
    }
}

