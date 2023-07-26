#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "structs.h"
#include "defines.h"

unsigned int get_objects_quantity(const int length, const int step, const unsigned int mod);

/**
 * Die
 * @param const char *const string
 * @return void
*/
void die(const char *const string)
{
    write(STDOUT_FILENO, "\033[?25h", 6);

    perror(string);
    exit(EXIT_FAILURE);
}

/**
 * Check attributes
 * @param const struct termios current_attributes
 * @return void
*/
void check_attributes(const struct termios current_attributes)
{
    if (tcgetattr(STDIN_FILENO, &con.check_attributes) == -1)
        die(strerror(errno));

    if (memcmp(&con.check_attributes, &current_attributes, sizeof(struct termios)) != 0)
    {
        printf("\r\n");
        printf("\033[1m[*]\033[0m Raw mode has not been enable/disable\r\n");
        printf("\033[1m[*]\033[0m Note: you may need reset the terminal\r\n");
    }
}

/**
 * Abuf free
 * @param struct abuf *ab
 * @return void
*/
void abuf_free(struct abuf *ab)
{
    free(ab->output_string);
}

/**
 * Abuf append
 * @param struct abuf *ab
 * @param const char *string
 * @param unsigned int length
 * @return int
*/
int abuf_append(struct abuf *ab, const char *string, const unsigned int length)
{
    char *new_string = realloc(ab->output_string, (ab->output_data_amount + length));
    if (new_string == NULL)
        return -1;

    memmove(&*(new_string + ab->output_data_amount), string, length);

    ab->output_string = new_string;
    ab->output_data_amount += length;

    return 0;
}


/**
 * Change cursor position
 * @param unsigned int row
 * @param unsigned int col
 * @return int
*/
int change_cursor_position(struct abuf *ab, const unsigned int row, const unsigned int col)
{
    char escape_sequence[BUFFER_LENGTH] = {'\0'};
    snprintf(escape_sequence, BUFFER_LENGTH, "\033[%d;%dH", row, col);

    if (abuf_append(ab, escape_sequence, strlen(escape_sequence)) == -1)
        return -1;

    return 0;
}

/**
 * Get cursor position
 * @param unsigned int *rows
 * @param unsigned int *cols
 * @return int
*/
int get_cursor_position(unsigned int *rows, unsigned int *cols)
{
    char buffer[BUFFER_LENGTH] = {'\0'};

    if (write(STDOUT_FILENO, "\033[6n", 4) == -1)
        return -1;

    unsigned int i = 0;
    for (; i < (BUFFER_LENGTH - 1); i++)
    {
        if (read(STDIN_FILENO, &*(buffer + i), 1) == -1)
            break;

        if (*(buffer + i) == 'R')
            break;
    }

    *(buffer + i) = '\0';

    if (*buffer != '\0' || *(buffer + 1) != '[')
        return -1;

    if (sscanf(&*(buffer + 2), "%u;%u", rows, cols) != 2)
        return -1;

    return 0;
}

/**
 * Get window size
 * @param unsigned int *rows
 * @param unsigned int *cols
 * @return int
*/
int get_window_size(unsigned int *rows, unsigned int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 1 || ws.ws_col == 0 || ws.ws_row == 0)
    {
        if (write(STDOUT_FILENO, "\033[999C\033[999R", 12) == -1)
            return -1;

        return get_cursor_position(rows, cols);
    }
    else
    {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    }

    return 0;
}

/**
 * Reset values
 * @param void
 * @return void
*/
void reset_values(void)
{
    od.directories_count = 0;
    od.files_count = 0;

    for (unsigned int i = 0; i < ELEMENTS_TOTAL_AMOUNT; i++)
    {
        for (unsigned int j = 0; j < NAME_LENGTH; j++)
        {
            od.directories[i][j] = '\0';
            od.files[i][j] = '\0';
        }
    }

    od.arraydf_count = 0;

    for (unsigned int i = 0; i < ELEMENTS_TOTAL_AMOUNT; i++)
        for (unsigned int j = 0; j < NAME_LENGTH; j++)
            od.arraydf[i][j] = '\0';
}

/**
 * Get slash index
 * @param const char *string
 * @param const unsigned int length
 * @return unsigned int
*/
unsigned int get_slash_index(const char *string, const unsigned int length)
{
    unsigned int slash_index = length;

    for (; *(string + slash_index) != '/' ; --slash_index)
        ;

    return slash_index;
}

/**
 * Address bar update
 * @param const unsigned int length
 * @param char *dest
 * @param const char *src
 * @return void
*/
void address_bar_update(const unsigned int length, char *dest, const char *src)
{
    unsigned int current_length_address_bar = strlen(dest);

    for (unsigned int i = 0; i < length; i++)
        *(dest + i) = '\0';

    unsigned int slash_index = get_slash_index(src, (od.current_buffer_length - 1));

    for (unsigned int i = 0; i <= slash_index; i++)
        *(dest + i) = *(src + i);

    if (current_length_address_bar != strlen(dest) && con.commit_address_bar_update == 0)
        con.commit_address_bar_update = 1;
}

/**
 * Split string
 * @param const char *string
 * @param const unsigned int length
 * @return void
*/
void split_string(const char *string, const unsigned int length)
{
    unsigned int index = 0;
    unsigned int n = get_objects_quantity(length, 16, 1);

    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < 16 && *(string + index) != '\0'; j++)
        {
            od.arraydf[od.arraydf_count][j] = *(string + index);
            ++index;
        }

        ++od.arraydf_count;
    }

    ++od.arraydf_count;
}

/**
 * Add files/dirs to arraydf
 * @param const unsigned int quantity
 * @param const unsigned int length
 * @param char strings[][length]
 * @return void
*/
void add_to_arraydf(const unsigned int quantity, const unsigned int length, char strings[][length])
{
    unsigned int str_len = 0;

    for (unsigned int i = 0; i < quantity; i++)
    {
        str_len = strlen(*(strings + i));
        if (str_len < 16)
        {
            for (unsigned int j = 0; j < str_len; j++)
                od.arraydf[od.arraydf_count][j] = strings[i][j];

            ++od.arraydf_count;
        }
        else
            split_string(*(strings + i), str_len);
    }
}

/**
 * Concatenate arrays
 * @param const unsigned int length
 * @param char src_1[][length]
 * @param char src_2[][length]
 * @return void
 * 
*/
void concatenate_arrays(const unsigned int length, char dirs[][length], char files[][length])
{
    add_to_arraydf(od.directories_count, length, dirs);
    add_to_arraydf(od.files_count, length, files);
}

/**
 * Get objects quantity
 * @param const int length
 * @param const int step
 * @return unsigned int 
*/
unsigned int get_objects_quantity(const int length, const int step, const unsigned int mod)
{
    int n = 0;
    int value = length;

    switch(mod)
    {
        case 0:
            for (; (value - step) > 0 ; ++n)
                value -= step;
        break;

        case 1:
            for (;value > 0; ++n)
                value -= step;
        break;
    }

    return n;
}