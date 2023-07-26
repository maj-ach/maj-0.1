#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Determine program status
 * @param void
 * @return void
*/
void determine_program_status(void)
{
    if (getuid() == 0)
    {
        printf("\n[!] The program was launched with\033[1m administrator\033[0m privileges\n");
        exit(EXIT_FAILURE);
    }
}