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

#include <stdio.h>
#include <string.h>

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

// Takes input from CLI arguments
int
input_arg_text(Text text, const int argc, const char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        strcat(text, argv[i]);
        if (i + 1 != argc) {
            const char *SPACE = " ";
            strcat(text, SPACE);
        }
    }
    return strlen(text);
}

// Takes input from CLI stdin
int
input_stdin_text(Text text) {
    int c, len = 0;
    while ((c = getchar()) != EOF) {
        text[len++] = c;
    }
    return len;
}

// Decides whether to take input from arguments or stdin, and does that
void
input_text(Text text, const int argc, const char *argv[]) {
    int len;
    if (argc > 1) {
        len = input_arg_text(text, argc, argv);
    } else {
        len = input_stdin_text(text);
    }
    if (text[len - 1] == '\n') {
        text[--len] = '\0';
    }
    text[len] = '\0';
}

// Copy string to string, but with every tab replaced with a number of spaces, and returns length
int
detab_strcpy(char dest[], const char src[]) {
    int len = 0, i = 0;
    while ((dest[len] = src[i++])) {
        if (dest[len] == '\t') {
            dest[len] = '\0';
            const char *TAB_SPACE = "    ";
            strcat(dest, TAB_SPACE);
            len += strlen(TAB_SPACE);
        } else {
            ++len;
        }
    }
    return len;
}

// Formats the given text into a list of lines
void
parse_lines(Lines *lines, Text text) {
    char *curr_line = text;
    while (curr_line) {
        char *next_line = strchr(curr_line, '\n');
        if (next_line) {
            *next_line = '\0';
        }
        printf("%s", lines->lines_arr[lines->lines_len]);
        int len = detab_strcpy(lines->lines_arr[lines->lines_len], curr_line);
        if (len > lines->max_len) {
            lines->max_len = len;
        }
        if (next_line) {
            *next_line = '\n';
        }
        curr_line = next_line ? next_line + 1 : NULL;
        ++(lines->lines_len);
    }
}

// Prints the text in a speech bubble
void
print_speech(Text text) {
    Lines lines = { 0 };
    parse_lines(&lines, text);
    printf(" ");
    for (int i = 0; i < lines.max_len + 2; ++i) {
        printf("_");
    }
    printf("\n");
    for (int i = 0; i < lines.lines_len; ++i) {
        printf("| %s", lines.lines_arr[i]);
        for (int j = 0, len = strlen(lines.lines_arr[i]); j < lines.max_len - len; ++j) {
            printf(" ");
        }
        printf(" |\n");
    }
    printf(" ");
    for (int i = 0; i < lines.max_len + 2; ++i) {
        printf("-");
    }
}

// Draws the text, speech bubble, and Squidward himself
void
draw_squid(Text text) {
    print_speech(text);
    printf("\n    \\   ___");
    printf("\n     \\ (.. )");
    printf("\n       -|-");
    printf("\n       /| |\\");
    printf("\n        | |");
    printf("\n       / | \\\n");
}

int
main(const int argc, const char *argv[]) {
    Text text;
    input_text(text, argc, argv);
    draw_squid(text);
    return 0;
}
