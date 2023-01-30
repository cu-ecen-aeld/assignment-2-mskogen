/*****************************************************************************
​*​ ​Copyright​ ​(C)​ ​2023 ​by​ Matthew Skogen
​*
​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​*​ ​forms​ ​is​ ​permitted​ ​as​ ​long​ ​as​ ​the​ ​files​ ​maintain​ ​this​ ​copyright.​ ​Users​ ​are
​*​ ​permitted​ ​to​ ​modify​ ​this​ ​and​ ​use​ ​it​ ​to​ ​learn​ ​about​ ​the​ ​field​ ​of​ ​embedded
​*​ ​software.​ ​Matthew Skogen ​and​ ​the​ ​University​ ​of​ ​Colorado​ ​are​ ​not​ ​liable​ ​for
​*​ ​any​ ​misuse​ ​of​ ​this​ ​material.
​*
*****************************************************************************/
/**
​*​ ​@file​ writer.c
​*​ ​@brief​ Application entry point
​*
​*​ ​@author​s ​Matthew Skogen
​*​ ​@date​ January 28 ​2023
*
*  Replacement for writer.sh, but:
*   1. Doesn't need to create directories that don't exist
*   2. Setup syslog logging using LOG_USER facility.
*   3. Use syslog to write a message "Writing <string> to <file>" at LOG_DEBUG
*       - string = text written to file
*       - file = file created by the script
*   4. Use syslog to log unexpected errors with LOG_ERR level
​*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    /* Open connection to system logger with default LOG_USER facility */
    openlog("writer", LOG_CONS, LOG_USER);

    /* Verify proper number of arguments passed in */
    if (argc != 3) {
        printf("ERROR: Invalid number of arguments %i\n", argc);
        printf("Usage: ./writer /path/to/new/file <write_string>\n");
        return 1;
    }

    /* First argument is a path to directory on filesystem */
    char *write_file=argv[1];

    /* Second argument is text string to be written to the write file */
    char *write_str=argv[2];

    /* 
     * Open a connection to the file, create new file if it doesn't exist and
     * overwrite file if it does exist.
     */
    FILE *p_file;
    int ret_val = 0;
    p_file = fopen(write_file, "w");

    if (p_file == NULL) {
        syslog(LOG_ERR, "Failed to open %s: %m\n", write_file);
        return 1;
    }

    /* File opened succesfully, write to file */
    syslog(LOG_DEBUG, "Writing '%s' to '%s'\n", write_str, write_file);
    ret_val = fprintf(p_file, "%s", write_str);

    if (ret_val < 0) {
        /* Error writing to file, check errno */
        syslog(LOG_ERR, "Failed to write to file: %m\n");
    } else if (ret_val != strlen(write_str)) {
        /* Failed to write all bytes to file */
        syslog(LOG_WARNING, "Only wrote %i bytes to file\n", ret_val);
    } else {
        /* Successful write */
        syslog(LOG_DEBUG, "Successfully wrote full string to file.\n");
    }

    /* Close out connection to the file descriptor if file was opened */
    if (p_file) {
        fclose(p_file);
    }

    /* Close out connection to system logger */
    closelog();

    return 0;
}