#include "state.h"
#include "asserts.h"
#include "state.c"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"
bool test_update_tail_board_1() {

    /*
    Board 1:
    ####################            ####################
    #                  #            #                  #
    # d>>D   *         #            #  d>D   *         #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  # ---------> #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    #                  #            #                  #
    ####################            ####################
    */

    // set up expected board
    game_state_t *expected = create_default_state();
    set_board_at(expected, 2, 2, ' ');
    set_board_at(expected, 2, 3, 'd');
    set_board_at(expected, 2, 4, '>');
    set_board_at(expected, 2, 5, 'D');
    expected->snakes->head_row = 2;
    expected->snakes->head_col = 5;
    expected->snakes->tail_row = 2;
    expected->snakes->tail_col = 3;
    save_board(expected, "unit-test-ref.snk");

    // set up actual board
    game_state_t *actual = create_default_state();
    set_board_at(actual, 2, 4, '>');
    set_board_at(actual, 2, 5, 'D');
    actual->snakes->head_row = 2;
    actual->snakes->head_col = 5;
    save_board(actual, "unit-test-in.snk");

    update_tail(actual, 0);
    save_board(actual, "unit-test-out.snk");



    free_state(expected);
    free_state(actual);

    return true;
}

int main(int argc, char *argv[]) {
    printf("hello");

    test_update_tail_board_1();
    return 0;
}