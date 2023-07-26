#define _DEFAULT_SOURCE // -D_DEFAULT_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#include "defines.h"
#include "addl_funcs.h"

/**
 * Write in array
 * @param unsigned int *count
 * @param const unsigned int length
 * @param char array[][length]
 * @param const char *cosnt string
*/
void write_in_array(unsigned int *count, const unsigned int length, char array[][length], const char *const string)
{
    unsigned int string_length = strlen(string);
    if (string_length < (length - 1) && *count < ELEMENTS_TOTAL_AMOUNT)
    {
        for (unsigned int i = 0; i < string_length; i++)
            array[*count][i] = *(string + i);

        ++(*count);
    }
}

/**
 * Read directory
 * @param DIR *directory_pointer
 * @return void
*/
void read_directory(DIR *directory_pointer)
{
    struct dirent *entry = NULL;
    while ((entry = readdir(directory_pointer)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            write_in_array(&od.directories_count, NAME_LENGTH, od.directories, entry->d_name);
            od.directories[od.directories_count - 1][strlen(entry->d_name)] = '/';
        }
        else
            write_in_array(&od.files_count, NAME_LENGTH, od.files, entry->d_name);
    }
}

/**
 * Open_file
 * @param char *address
 * @return unsigned int
*/
unsigned int open_file(char *address)
{
    FILE *file_pointer = NULL;
    unsigned int return_value = 0;

    *(address + (strlen(address) - 1)) = '\0'; 

    if ((file_pointer = fopen(address, "rb")) != NULL)
    {
        if (fclose(file_pointer) != 0)
            die(strerror(errno));

        return_value = 1;

        char command[100] = {"\0"};
        snprintf(command, sizeof(command), "%s %s", con.editor_name, address);

        if (system(command) == -1)
            die(strerror(errno));
    }
    else
        return_value = 0;

    *(address + strlen(address)) = '/';

    return return_value;
}

/**
 * Open directory
 * @param char *address
 * @return unsigned int
*/
unsigned int open_directory(char *address)
{
    DIR *directory_pointer = NULL;
    unsigned int return_value = 0;

    reset_values();

    if ((directory_pointer = opendir(address)) != NULL)
    {
        return_value = 1;

        read_directory(directory_pointer);
        concatenate_arrays(NAME_LENGTH, od.directories, od.files);

        closedir(directory_pointer);
    }
    else
        return_value = 0;

    return return_value;
}

/**
 * Open object
 * @param char *address
 * @return void
*/
void open_object(char *address)
{
    if (con.commit_address_bar_update == 1)
    {
        con.commit_address_bar_update = 0;

        if ((con.directory_state = open_directory(address)) == 0)
            con.file_state = open_file(address);
    }
}

/**
 * Call open object
 * @param void
 * @return void
*/
void call_open_object(void)
{
    address_bar_update(LENGTH, od.address_bar, od.buffer);
    open_object(od.address_bar);
}