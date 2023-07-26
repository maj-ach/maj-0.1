#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "structs.h"
#include "defines.h"
#include "addl_funcs.h"
#include "autocomplete.h"

/**
 * Buffer append
 * @param char *buffer
 * @param const char character
 * @param unsigned int length
 * @param const unsigne int size
 * @return unsigned int
*/
unsigned int buffer_append(char *buffer, const char character, unsigned int length, const unsigned int size)
{
    if (character == 127 && length > 1)
    {
        *(buffer + (length - 1)) = '\0';
        --length;
    }
    else if (character != 127 && (length + 1) < size)
    {
        if (con.directory_state == 1)
        {
            *(buffer + length) = character;
            ++length;
        }
    }

    return length;
}

/**
 * Keypress handler
 * @param char character
 * @return int
*/
int keypress_handler(char character)
{
    if (character == '/' && *(od.buffer + (strlen(od.buffer) - 1)) == '/')
        return -1;

    switch (character)
    {
        case 32: // Space
            return -1;

        case 17: // ^Q
            write(STDOUT_FILENO, "\033[?25h", 6);
            exit(EXIT_SUCCESS);

        case 13: // Carrage Return
            return -1;

        case 12: // Form Feed
            return -1;

        case 11: // Vertical Tabulation && ^K
            if (od.page_number != 0)
                --od.page_number;
            return -1;

        case 10: // Line Feed && ^J
            ++od.page_number;

            con.columns_quantity = get_objects_quantity(con.screencols, 20, 0);

            unsigned int result = get_objects_quantity(od.arraydf_count, ((con.screenrows - 8) * con.columns_quantity), 0);

            if (od.page_number > result)
                --od.page_number;

            return -1;

        case 9: // Horizontal Tabulation
            call_autocomplete();
            return -1;

        case 8: // Backspace
            return -1;

        default:
            break;
    }

    return 0;
}

/**
 * Input
 * @param char *key_i
 * @return void
*/
void input(char *key_i)
{
    int value = 0;

    while ((value = read(STDIN_FILENO, key_i, 1)) != 1)
    {
        if (value == -1 && errno != 0)
            die(strerror(errno));
    }
}

/**
 * Call input
 * @param void
 * @return void
*/
void call_input(void)
{
    char key = '\0';

    input(&key);

    if (keypress_handler(key) != -1)
        od.current_buffer_length = buffer_append(od.buffer, key, strlen(od.buffer), LENGTH);
}