/* 
 *  Replacement for writer.sh, but:
 *  1. Doesn't need to create directories that don't exist
 *  2. Setup syslog logging using LOG_USER facility.
 *  3. Use syslog to write a message "Writing <string> to <file>" at LOG_DEBUG level
 *      - string = text written to file
 *      - file = file created by the script
 *  4. Use syslog to log unexpected errors with LOG_ERR level
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    /* exit value 1, print statement for missing parameters */
    if (argc != 3) {
        printf("ERROR: Invalid number of arugments %i\n", argc);
        printf("Usage: writer /path/to/new/file <write_string>\n");
        return 1;
    }

    /* first argument is a path to directoy on filesystem */
    char *write_file=argv[1];

    /* second argument is text string to be search for within files */
    char *write_str=argv[2];

    /* overwrite OR create new file if it doesn't exist */
    FILE *p_file;
    int ret_val = 0;
    p_file = fopen(write_file, "w");

    if (p_file == NULL) {
        printf("fopen failed: %s\n", strerror(errno));
    } else {
        ret_val = fprintf(p_file, "%s", write_str);
    }

    if (ret_val < 0) {
        /* Error writting, check errno */
        printf("write to file failed: %s\n", strerror(errno));
    } else if (ret_val == 0) {
        /* Nothing written */
        printf("nothing written to file %s\n", write_file);
    } else {
        /* Successful write */
        printf("successfully wrote '%s' to %s\n", write_str, write_file);
    }

    fclose(p_file);

    return 0;
}