#include <string.h>

#include "structs.h"
#include "defines.h"

const char *const allowed_flags[] = {"--help", "--version", "--ed0nano", "--ed1code", "--ed2vim"};
const unsigned int allowed_flags_length = sizeof(allowed_flags) / sizeof(*allowed_flags);

const char *const allowed_commands[] = {"--help", "--version"};
const unsigned int allowed_commands_length = sizeof(allowed_commands) / sizeof(*allowed_commands);

const char *const allowed_editors[] = {"--ed0nano", "--ed1code", "--ed2vim"};
const unsigned int allowed_editors_length = sizeof(allowed_editors) / sizeof(*allowed_editors);

void init_base_editor_name(char *editor, const char *const base_editor)
{
    for (unsigned int i = 0; i < strlen(base_editor); i++)
        *(editor + i) = *(base_editor + i);
}

/**
 * Init array
 * @param char *array
 * @param const unsigned int length
 * @return void
*/
void init_array(char *array, const unsigned int length)
{
    for (unsigned int i = 0; i < length; i++)
        *(array + i) = '\0';
}

/**
 * Init address
 * @param char *dest
 * @param const unsigned int dest_length
 * @param char *src
 * @param const unsigned int src_length
 * @return void
*/
unsigned int init_address(char *dest, const unsigned int dest_length, char *src, const unsigned int src_length)
{
    init_array(dest, dest_length);
    
    if ((src_length + 1) < dest_length)
    {
        unsigned int n = 0;

        for (; n < src_length; n++)
            *(dest + n) = *(src + n);

        return n;
    }

    return 0;
}

/**
 * Init two-dimensional array
 * @param const unsigned int size
 * @param const unsigned int length
 * @param char array[size][length]
 * @return void
*/
void init_two_dim_array(const unsigned int size, const unsigned int length, char array[size][length])
{
    for (unsigned int i = 0; i < size; i++)
        for (unsigned int j = 0; j < length; j++)
            array[i][j] = '\0';
}

/**
 * Init values
 * @param void
 * @return void
*/
void init_values(void)
{
    od.current_address_bar_length = init_address(od.address_bar, LENGTH, ADDRESS_BAR, strlen(ADDRESS_BAR));
    od.current_buffer_length = init_address(od.buffer, LENGTH, od.address_bar, strlen(od.address_bar));

    con.screenrows = 0;
    con.screencols = 0;

    con.directory_state = 1;
    con.file_state = 0;

    con.commit_address_bar_update = 1;

    con.columns_quantity = 0;

    od.directories_count = 0;
    od.files_count = 0;

    init_two_dim_array(SIZE_ARRAY, NAME_LENGTH, od.directories);
    init_two_dim_array(SIZE_ARRAY, NAME_LENGTH, od.files);

    init_two_dim_array(ELEMENTS_TOTAL_AMOUNT, NAME_LENGTH, od.arraydf);

    od.arraydf_count = 0;

    od.page_number = 0;
    od.max_page_number = 0;

    init_array(con.editor_name, EDITOR_NAME_LENGTH);
    init_base_editor_name(con.editor_name, BASE_EDITOR);
}