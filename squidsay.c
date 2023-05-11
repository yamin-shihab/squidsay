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

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// `getdelim` with optional delimiter inclusion and
// predicates that dictate what characters to strip from each side.
//
// Returns -1 on failure, the cause of which can be deduced by checking
// the EOF status of `file`, otherwise,
// by checking the error of `file`, or if none of the previous apply,
// by checking `errno`.
//
// On success, returns the
// count of characters in `*line` excluding the `'\0'` terminator
// and sets `*line` and `*line_capacity` as described.
static ssize_t
getdelim_stripped(
        // `NULL` or pointer to buffer of size `*line_capacity`.
        // The pointer must be able to be passed to `free`.
        // `'\0'` terminated on successful output.
        char** line,
        // Pointer to the capacity of `*line`.
        // `*line_capacity` will not be read if `line` is `NULL`.
        // `*line_capacity` is updated to reflect the capacity of `*line`
        // as it changes during this procedure.
        size_t* line_capacity,
        int delimiter,
        FILE* file,
        int (*remove_left)(int),
        int (*remove_right)(int),
        // Whether or not to include `delimiter` at the end of `*line` and in the returned size if it was found.
        bool keep_delimiter)
{
    size_t line_count = 0;

    int character = getc(file);

    if (character == EOF) return -1;

    if (!*line) {
        void* buffer = malloc(100);
        if (!buffer) {
            ungetc(character, file);
            return -1;
        }
        *line = buffer;
        *line_capacity = 100;
    }

    // Strip left:
    while (character != EOF && character != delimiter && (*remove_left)(character)) 
        character = getc(file);

    // Collect characters:
    while (character != EOF && character != delimiter) {
        // Grow:
        if (line_count == *line_capacity) {
            void* buffer = realloc(*line, *line_capacity + 100);
            if (!buffer) {
                ungetc(character, file);
                return -1;
            }
            *line = buffer;
            *line_capacity += 100;
        }

        (*line)[line_count++] = character;
        character = getc(file);
    }

    // Strip right:
    while (line_count > 0 && (*remove_right)((*line)[line_count - 1]))
        line_count -= 1;

    // Resize `*line` to be just enough to fit
    // the delimiter if desired and found
    // and `'\0'`:

    keep_delimiter = keep_delimiter && character == delimiter;

    size_t size = line_count + (keep_delimiter ? 2 : 1);

    void* buffer = realloc(*line, size);
    if (buffer)
        *line_capacity = size;
    else if (size > *line_capacity) 
        return -1;
    *line = buffer;

    if (keep_delimiter) (*line)[line_count++] = delimiter;
    (*line)[line_count] = '\0';

    return line_count;
}

struct FatChar {
    char* buffer;
    size_t count;
};

static struct FatChar
getcontents(struct FatChar line)
{
    ssize_t count = getdelim_stripped(
            &line.buffer,
            &line.count,
            '\n',
            stdin,
            &isspace,
            &isspace,
            false);

    if (count == -1)
        return (struct FatChar){ NULL, 0 };

    return (struct FatChar){ line.buffer, count };
}

int
main(void)
{
    struct FatChar* lines = NULL;
    size_t lines_count = 0;
    size_t lines_capacity = 0;

    size_t line_longest = 0;

    while (true) {
        struct FatChar line = getcontents((struct FatChar){ NULL, 0 });

        if (!line.buffer) {
            if (feof(stdin)) {
                // Draw top border:
                putchar(' ');
                for (size_t i = 0; i < line_longest + 2; ++i)
                    putchar('_');
                putchar('\n');

                // Draw contents:
                for (size_t line = 0; line < lines_count; ++line) {
                    fputs("| ", stdout);
                    fputs(lines[line].buffer, stdout);
                    for (size_t i = 0; i < line_longest - lines[line].count; ++i)
                        putchar(' ');
                    puts(" |");
                }

                // Draw bottom border:
                putchar(' ');
                for (size_t i = 0; i < line_longest + 2; ++i)
                    putchar('-');

                // Draw squid:
                puts("\n\
    \\   ___\n\
     \\ (.. )\n\
       -|-\n\
       /| |\\\n\
        | |\n\
       / | \\");

                return 0;
            }

            int error = ferror(stdin);
            if (error) return error;

            perror(NULL);
            return errno;
        }

        if (line.count > line_longest) line_longest = line.count;

        // Grow:
        if (lines_count == lines_capacity) {
            size_t new_capacity = lines_capacity + 100 * sizeof(struct FatChar);

            void* buffer = realloc(lines, new_capacity);
            if (!buffer) {
                perror(NULL);
                return errno;
            }

            lines = buffer;
            lines_capacity = new_capacity;
        }

        lines[lines_count++] = line;
    }
}
