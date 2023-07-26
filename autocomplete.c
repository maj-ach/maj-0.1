#include <string.h>

#include "structs.h"
#include "defines.h"
#include "addl_funcs.h"

/**
 * Buffer autocomplete
 * @param char *buffer
 * @param const unsigned int buffer_length
 * @param const char *string
 * @param const unsigned int string_length
 * @return unsigned int
*/
unsigned int buffer_autocomplete(char *buffer, const unsigned int buffer_length, const char *string, const unsigned int string_length)
{
    unsigned int slash_index = get_slash_index(buffer, buffer_length);
    ++slash_index;

    for (unsigned int i = slash_index; i < LENGTH; i++)
        *(buffer + i) = '\0';
    
    for (unsigned int i = 0; i < string_length; i++)
    {
        *(buffer + slash_index) = *(string + i);
        ++slash_index;
    }

    return slash_index;
}

/**
 * Get longest string index
 * @param const unsigned int autocomp_length
 * @param const unsigned int length
 * @param char array[][length]
 * @return unsigned int
*/
unsigned int get_longest_string_index(const unsigned int autocomp_length, const unsigned int length, char array[][length])
{
    unsigned int longest_string_index = 0;
    for (unsigned int i = 0; i < autocomp_length; i++)
    {
        if (strlen(*(array + longest_string_index)) <= strlen(*(array + i)))
            longest_string_index = i;
    }

    return longest_string_index;
}

/**
 * Get common part
 * @param const unsigned int length
 * @param char autocomlete_strings[][length]
 * @param const unsigned int autocomp_length
 * @param const unsigned int autocomp_length
 * @param char *common_part
 * @return unsigned int
*/
unsigned int get_common_part(const unsigned int length, char autocomplete_strings[][length], const unsigned int autocomp_length, char *common_part)
{
    unsigned int common_part_length = 0;
    unsigned int longest_string_index = get_longest_string_index(autocomp_length, NAME_LENGTH, autocomplete_strings);

    unsigned int longest_string_length = strlen(*(autocomplete_strings + longest_string_index));

    for (unsigned int i = 0; i < longest_string_length; i++)
    {
        for (unsigned int j = 0; j < autocomp_length; j++)
        {
            if (strncmp(*(autocomplete_strings + longest_string_index), *(autocomplete_strings + j), i) == 0)
                continue;
            else
                return common_part_length;
        }

        *(common_part + common_part_length) = autocomplete_strings[longest_string_index][i];
        ++common_part_length;
    }

    return common_part_length;
}

/**
 * Get autocomplete strings
 * @param const unsigned int length
 * @param char autocomplete_stringsp[][length]
 * @param const char *string
 * @param const unsigned int strings_length
 * @return unsigned int
*/
unsigned int get_autocomplete_strings(const unsigned int length, char autocomplete_strings[][length], const char *string, const unsigned int string_length)
{
    int count = 0;
    unsigned int autocomp_length = 0;

    for (unsigned int i = 0; i < od.arraydf_count; i++)
    {
        for (unsigned int j = 0; j < string_length; j++)
        {
            if (od.arraydf[i][j] == *(string + j))
                count = 0;
            else
            {
                count = -1;
                break;
            }
        }

        if (count == 0)
        {
            for (unsigned int n = 0; n < strlen(*(od.arraydf + i)); n++)
                autocomplete_strings[autocomp_length][n] = od.arraydf[i][n];

            ++autocomp_length;
        }

        else
            count = 0;
    }

    return autocomp_length;
}


/**
 * Get string
 * @param char *string
 * @return strign_length
*/
unsigned int get_string(char *string)
{
    unsigned int slash_index = get_slash_index(od.buffer, od.current_buffer_length);
    unsigned int string_length = 0;

    ++slash_index;

    for (; *(od.buffer + slash_index) != '\0'; string_length++)
    {
        *(string + string_length) = *(od.buffer + slash_index);
        ++slash_index;
    }

    return string_length;
}

/**
 * Call autocomplete
 * @param void
 * @return unsigned int
*/
void call_autocomplete(void)
{
    char string[NAME_LENGTH] = {'\0'};
    unsigned int string_length = get_string(string);

    if (string_length > 0)
    {
        char autocomplete_strings[SIZE_ARRAY][NAME_LENGTH] = {'\0'};
        unsigned int autocomp_length = get_autocomplete_strings(NAME_LENGTH, autocomplete_strings, string, string_length);

        char common_part[NAME_LENGTH] = {'\0'};
        unsigned int common_part_length = get_common_part(NAME_LENGTH, autocomplete_strings, autocomp_length, common_part);

        od.current_buffer_length = buffer_autocomplete(od.buffer, od.current_buffer_length, common_part, common_part_length);
    }
}