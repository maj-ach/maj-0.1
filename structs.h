#include <termios.h>
#include "defines.h"

struct configs
{
    struct termios original_attributes;
    struct termios new_attributes;
    struct termios check_attributes;

    unsigned int screenrows;
    unsigned int screencols;

    unsigned int directory_state;
    unsigned int file_state;
    unsigned int commit_address_bar_update;

    unsigned int columns_quantity;

    char editor_name[EDITOR_NAME_LENGTH];
};

struct configs con;

struct output_data
{
    char address_bar[LENGTH];
    char buffer[LENGTH];

    unsigned int current_address_bar_length;
    unsigned int current_buffer_length;

    char directories[SIZE_ARRAY][NAME_LENGTH];
    char files[SIZE_ARRAY][NAME_LENGTH];

    unsigned int directories_count;
    unsigned int files_count;

    char arraydf[ELEMENTS_TOTAL_AMOUNT][NAME_LENGTH];

    unsigned int arraydf_count;

    unsigned int page_number;
    unsigned int max_page_number;
};

struct output_data od;

struct abuf
{
    char *output_string;
    unsigned int output_data_amount;
};