#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "validator.h"

/**
 * Main
 * @param void
 * @return int
*/
int main(const int argc, const char *const argv[])
{
    determine_program_status();
    init_values();

    if (argc > 1)
    {
        validate_flags(argc, argv);
        commands_handler(argc, argv);
    }

    enable_raw_mode();

    while (1)
    {
        call_open_object();
        call_refresh_screen();
        call_input();
    }

    return EXIT_SUCCESS;
}