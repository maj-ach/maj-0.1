#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "defines.h"

void help_command(void);
void version_command(void);
void apply_flags(const unsigned int size, const char *const names[]);

void (*commands_store[])(void) = {help_command, version_command};

/**
 * Validate flags
 * @param const unsigne int length
 * @param const char *const flags[]
 * @return void
*/
void validate_flags(const unsigned int length, const char *const flags[])
{
    int incorrect_flag_index = -1;

    for (unsigned int i = 1; i < length; i++)
    {
        for (unsigned int j = 0; j < allowed_flags_length; j++)
        {
            if (strcmp(*(flags + i), *(allowed_flags + j)) == 0)
            {
                incorrect_flag_index = -1;
                break;
            }
            else
                incorrect_flag_index = i;
        }

        if (incorrect_flag_index != -1)
        {
            printf("\n[!] Error: \'%s\' is incorrect flag\n", *(flags + incorrect_flag_index));
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Commands handler
 * @param const unsigned int count
 * @param const char *const commands[]
 * @return void
*/
void commands_handler(const unsigned int count, const char *const commands[])
{
    for (unsigned int i = 1; i < count; i++)
        for (unsigned int j = 0; j < allowed_commands_length; j++)
            if (strcmp(*(commands + i), *(allowed_commands + j)) == 0)
                commands_store[j]();

    apply_flags(count, commands);
}

/**
 * Init editor name
 * @param const char *const name
 * @return void
*/
void init_editor_name(const char *const name)
{
    for (unsigned int i = 0; i < EDITOR_NAME_LENGTH; i++)
        *(con.editor_name + i) = '\0';

    for (unsigned int i = 0; i < strlen(name); i++)
        *(con.editor_name + i) = *(name + i);
}

/**
 * Apply flags
 * @param const unsigned int size
 * @param const char *const names[]
 * @return void
*/
void apply_flags(const unsigned int size, const char *const names[])
{
    for (unsigned int i = 0; i < size; i++)
    {
        for (unsigned int j = 0; j < allowed_editors_length; j++)
        {
            if (strcmp(*(names + i), *(allowed_editors + j)) == 0)
            {
                switch (j)
                {
                    case 0:
                        init_editor_name(NANO);
                        break;
                    case 1:
                        init_editor_name(CODE);
                        break;
                    case 2:
                        init_editor_name(VIM);
                        break;
                }
                return;
            }
        }
    }
}

/**
 * Help command
 * @param void
 * @return void
*/
void help_command(void)
{
    printf("\n[*] Help command invoked\n");
    exit(EXIT_SUCCESS);
}

/**
 * Version command
 * @param void
 * @return void
*/
void version_command(void)
{
    printf("\n[*] Version command invoked\n");
    printf("\n[*] App version: %s\n", APP_VERSION);

    printf("\n");
    exit(EXIT_SUCCESS);
}


