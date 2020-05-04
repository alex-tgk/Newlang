#include "input.h"
#include <stdlib.h>

char* p;

char *read_file(char *path) {
    // Open and read the file.
    FILE *fp = fopen(path, "r");
    if (!fp)
        printf("cannot open %s", path);

    int filemax = 10 * 1024 * 1024;
    char *buf = malloc(filemax);
    int size = fread(buf, 1, filemax - 2, fp);
    if (!feof(fp))
        printf("file too large");

    // Make sure that the string ends with "\n\0".
    if (size == 0 || buf[size - 1] != '\n')
        buf[size++] = '\n';
    buf[size] = '\0';
    p = buf;
}

char *get_buffer(){
    return p;
}