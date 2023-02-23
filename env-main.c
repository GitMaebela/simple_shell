#include "main.h"



/**
* setenv
* @name:
* @value:
* @overwrite:
*/
int setenv(const char *name, const char *value, int overwrite) 
{
    int result = 0;

    if (!overwrite && getenv(name) != NULL) 
    {
        fprintf(stderr, "setenv: Variable %s already exists.\n", name);
        result = -1;
    } 
    else 
    {
        result = setenv(name, value, 1);
        if (result != 0) 
        {
            fprintf(stderr, "setenv: Failed to set variable %s.\n", name);
        }
    }
    return result;
}



/**
* unsetenv
* @name:
*/
int unsetenv(const char *name) 
{
    int result = unsetenv(name);
    if (result != 0) 
    {
        fprintf(stderr, "unsetenv: Failed to unset variable %s.\n", name);
    }

    return result;
}



// /**
// * main
// * @argc:
// * @argv:
// */
// int main(int argc, char** argv) 
// {
//     process_args(argc, argv);
//     return 0;
// }