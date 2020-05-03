#include "input.h"
#include "error.h"
#include <stdlib.h>
#include <unistd.h>

FILE *file;
char* _filename;

void open_file()
{
    if (access(_filename, F_OK ) == -1) {
        printf("Input: File doesn't exist!");
    }

    file = fopen(_filename, "r");
}

void set_file(char* filename) {
    _filename = filename;
}

char* get_filename() {
    return _filename;
}

FILE *get_file()
{
    return file;
}

void close_file()
{
    fclose(file);
}