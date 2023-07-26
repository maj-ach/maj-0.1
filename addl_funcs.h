#include <termios.h>

void die(const char *const string);
void check_attributes(const struct termios check_attributes);

void abuf_free(struct abuf *ab);
int abuf_append(struct abuf *ab, const char *string, const unsigned int length);

int get_window_size(unsigned int *rows, unsigned int *cols);
int change_cursor_position(struct abuf *ab, const unsigned int row, const unsigned int col);

void reset_values(void);
void address_bar_update(const unsigned int length, char *dest, const char *src);

void concatenate_arrays(const unsigned int length, char dirs[][length], char files[][length]);

unsigned int get_objects_quantity(const int length, const int step, const unsigned int mod);
unsigned int get_slash_index(const char *string, const unsigned int length);