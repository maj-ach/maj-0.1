#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "structs.h"
#include "addl_funcs.h"

/**
 * Disable raw mode
 * @param void
 * @return void
 */
void disable_raw_mode(void)
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &con.original_attributes) == -1)
        die(strerror(errno));

    check_attributes(con.original_attributes);
}

/**
 * Enable raw mode
 * @param void
 * @return void
 */
void enable_raw_mode(void)
{
    if (tcgetattr(STDIN_FILENO, &con.original_attributes) == -1)
        die(strerror(errno));

    atexit(disable_raw_mode);

    con.new_attributes = con.original_attributes;

    con.new_attributes.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    con.new_attributes.c_oflag &= ~(OPOST);
    con.new_attributes.c_cflag |= (CS8);
    con.new_attributes.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    con.new_attributes.c_cc[VMIN] = 0;
    con.new_attributes.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &con.new_attributes) == -1)
        die(strerror(errno));

    check_attributes(con.new_attributes);
}