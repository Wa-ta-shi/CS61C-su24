#include "state.h"
#include "asserts.h"
#include "state.c"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"
bool test_read_line_1() {

    FILE *fp = fopen("01-simple-in.snk", "r");
    char *expected = "";
    char *actual = read_line(fp);
    fclose(fp);

    if (strcmp(expected, actual) != 0) {
        free(actual);
        return false;
    }
    free(actual);
    return true;
}

int main(int argc, char *argv[]) {
    printf("hello");

    test_read_line_1();
    return 0;
}