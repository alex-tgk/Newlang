#include <stdio.h>
#include "input.h"
#include "lexer.h"
#include <stdlib.h>
#include <errno.h>


int main() {
    read_file("../test.nl");
    Tokenize(get_buffer());
    OutputTokens();
    return 0;
}
