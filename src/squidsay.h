/*  _________________________________________________
 * | Like cowsay, but with the sexiest guy around... |
 * |              Squidward Tentacles                |
 *  -------------------------------------------------
 *     \   ___
 *      \ (.. )
 *        -|-
 *        /| |\
 *         | |
 *        / | \
 */

#ifndef SQUIDSAY
#define SQUIDSAY

// Maximum amounts of text in bytes
#define MAX_LINES 256
#define MAX_LINE_LEN 256

// What the inputted text is stored in
typedef char Text[MAX_LINES * MAX_LINE_LEN];

// A formatted list of the lines of the text and the longest line
typedef struct {
    char lines_arr[MAX_LINES][MAX_LINE_LEN];
    int lines_len;
    int max_len;
} Lines;

// Decides whether to take input from arguments or stdin, and does that
void
input_text(Text text, const int argc, const char *argv[]);

// Takes input from CLI arguments
int
input_arg_text(Text text, const int argc, const char *argv[]);

// Takes input from CLI stdin
int
input_stdin_text(Text text);

// Draws the text, speech bubble, and Squidward himself
void
draw_squid(Text text);

// Prints the text in a speech bubble
void
print_speech(Text text);

// Formats the given text into a list of lines
void
parse_lines(Lines *lines, Text text);

// Copy string to string, but with every tab replaced with a number of spaces, and returns length
int
detab_strcpy(char dest[], const char src[]);

#endif
