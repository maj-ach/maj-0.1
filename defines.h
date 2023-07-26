#include <stddef.h>

#define LENGTH 512
#define BUFFER_LENGTH 32

#define ELEMENTS_TOTAL_AMOUNT 2000
#define SIZE_ARRAY 1000
#define NAME_LENGTH 256
#define EDITOR_NAME_LENGTH 5

#define ABUF_INIT {NULL, 0}
#define ADDRESS_BAR "/home/baddad/"

#define APP_VERSION "0.1"

#define BASE_EDITOR "nano"
#define NANO "nano"
#define CODE "code"
#define VIM "vim"

extern const char *const allowed_flags[];
extern const unsigned int allowed_flags_length;

extern const char *const allowed_commands[];
extern const unsigned int allowed_commands_length;

extern const char *const allowed_editors[];
extern const unsigned int allowed_editors_length;