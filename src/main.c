#include <stdio.h>
#include "input.h"
#include "lexer.h"
#include <stdlib.h>
#include <errno.h>

int main() {
    set_file("../test.nl");
    open_file();
    LexOpenFile();
    OutputTokens();
    close_file();
    return 0;
}
