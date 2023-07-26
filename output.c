#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#include "defines.h"
#include "addl_funcs.h"

/**
 * Draw objects
 * @param struct abuf *ab
 * @param const unsigned int length
 * @param const unsigned int count
 * @param char array[][length]
 * @return void
*/
void draw_objects(struct abuf *ab, const unsigned int length, const unsigned int count, char array[ELEMENTS_TOTAL_AMOUNT][length])
{
    unsigned int column_position = 0;
    unsigned int index = 0;

    if ((con.columns_quantity = get_objects_quantity(con.screencols, 20, 0)) > 5)
        con.columns_quantity = 5;

    const unsigned int page_step = (con.screenrows - 8) * con.columns_quantity;

    if (page_step < od.arraydf_count && (od.page_number * page_step) < od.arraydf_count)
        index = od.page_number * page_step;

    for (unsigned int i = 0; i < con.columns_quantity; i++)
    {
        for (unsigned int j = 8; j < con.screenrows && index < count; j++)
        {
            change_cursor_position(ab, j, column_position);
            abuf_append(ab, *(array + index), strlen(*(array + index)));
            ++index;
        }

        if (index >= od.arraydf_count)
            break;
        else
            column_position += 20;
    }
}

/**
 * Draw page number
 * @param struct abuf *ab
 * @param const unsigned int page_number
 * @return void
*/
void draw_page_number(struct abuf *ab, const unsigned int page_number)
{
    change_cursor_position(ab, 4, ((con.screencols / 3) - 2));

    char line[10] = { '\0' };
    snprintf(line, sizeof(line), "%d", page_number);

    abuf_append(ab, line, strlen(line));
}

/**
 * Draw address bar
 * @param struct abuf *ab
 * @param const char *address_bar
 * @param const unsigned int length
 * @return int
*/
int draw_address_bar(struct abuf *ab, const char *address_bar, const unsigned int length)
{
    if (change_cursor_position(ab, 4, (con.screencols / 3)) == -1)
        return -1;

    if (con.directory_state == 1 || con.file_state == 1)
        abuf_append(ab, "\033[32m", 5);

    else if (con.directory_state == 0 && con.file_state == 0)
        abuf_append(ab, "\033[31m", 5);

    if (abuf_append(ab, address_bar, length) == -1)
        return -1;

    abuf_append(ab, "\033[0m", 4);

    return 0;
}

/**
 * Draw rows
 * @param struct abug *ab
 * @return void
*/
int draw_rows(struct abuf *ab)
{
    if (draw_address_bar(ab, od.buffer, od.current_buffer_length) == -1)
        return -1;

    draw_page_number(ab, od.page_number);

    draw_objects(ab, NAME_LENGTH, od.arraydf_count, od.arraydf);

    return 0;
}

/**
 * Refresh screen
 * @param void
 * @return void
*/
void refresh_screen(void)
{
    struct abuf ab = ABUF_INIT;

    abuf_append(&ab, "\033[2J", 4);
    abuf_append(&ab, "\033[H", 3);
    abuf_append(&ab, "\033[?25l", 6);

    if (draw_rows(&ab) == -1)
        die(strerror(errno));

    if (write(STDOUT_FILENO, ab.output_string, ab.output_data_amount) == -1)
        die(strerror(errno));

    abuf_free(&ab);
}

/**
 * Call refresh screen
 * @param void
 * @return void
*/
void call_refresh_screen(void)
{
    if (get_window_size(&con.screenrows, &con.screencols) == -1)
        die(strerror(errno));

    if (con.screenrows >= 8 && con.screencols >= 60)
        refresh_screen();
    else if (write(STDOUT_FILENO, "\033[2J", 4) == -1)
        die(strerror(errno));
}