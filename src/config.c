// file : config.c 
// date : 27-06-2026
// std  : c23 

// include the header files. 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "config.h"
#include "protocol.h" // for BUF_SIZE
// initialise the config path to NULL. 
// this is a variable. 
char *CONFIG_PATH = NULL;

// define the config load function.
int config_load(PomData *data) {
    // allocate BUF_SIZE amount of data to the path. 
    CONFIG_PATH = malloc(BUF_SIZE);
    // use snprintf to store to config path. 
    snprintf(CONFIG_PATH, (size_t)(BUF_SIZE), "%s/.config/pomoc/config.ini", getenv("HOME"));

    // open the config file. 
    FILE *file = fopen(CONFIG_PATH, "r");
    // check if the fopen fails. if it fails, fallback to default value. 
    if (!file) {
        // set default values for focus timer. 
        data->focus_hr = 0;
        data->focus_min = 25;
        data->focus_sec = 0;

        // set default values for break timer. 
        data->break_min = 5;
        data->break_sec = 0;
        return EXIT_SUCCESS;
    }
    // if fopen succeeds, then we need to read through the config file. 
    // assuming each line is 256 bytes long (that is BUF_SIZE) (don't provide a malformed config.ini)
    // we need to while through each line. 
    char line[BUF_SIZE];

    // fgets : it reads in at most one less than size characters from stream and stores them into the buffer. Reading stops after an EOF or a newline. If a newline is read, it is stored into the buffer. A terminating null byte ('\0') is stored after the last character in the buffer. 
    while (fgets(line, sizeof(line), file)) {
        // if the line starts with # or is a newline, we skip it. 
        if (line[0] == '#' || line[0] == '\n' || (line[0] == ' ' && line[1] == '#')) continue;

        // initialise the hr, min and sec. 
        int fhr = 0, fmin = 25, fsec = 0, bmin = 5, bsec = 0;

        if (sscanf(line, "focus = %d:%d:%d", &fhr, &fmin, &fsec) == 3) {
            data->focus_hr = fhr;
            data->focus_min = fmin;
            data->focus_sec = fsec;
        } else if  (sscanf(line, "break = %d:%d", &bmin, &bsec) == 2){
            data->break_min = bmin;
            data->break_sec = bsec;
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}
