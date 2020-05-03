#ifndef NEWLANG_INPUT_H
#define NEWLANG_INPUT_H

#include <stdio.h>

void open_file();

FILE *get_file();

void close_file();

void set_file(char *);

char *get_filename();

#endif
