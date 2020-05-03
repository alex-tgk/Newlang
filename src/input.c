#include "input.h"
#include <io.h>

FILE *file;
char* filename;

void open_file()
{
    if (access(filename, F_OK ) == -1) {
        printf("Input: File doesn't exist!");
    }

    file = fopen(filename, "r");
}

void set_file(char* file) {
    filename = file;
}

char* get_filename() {
    return filename;
}

FILE *get_file()
{
    return file;
}

void close_file()
{
    fclose(file);
}