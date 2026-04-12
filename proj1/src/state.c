#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);
#define MAX_LINE 256

static void malloc_fail() {
  printf("malloc fail");
  exit(0);
}

static void free_game_state_t(game_state_t *state) {
  for (unsigned int i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }
  free(state->snakes);
  free(state->board);
  free(state);
}

char **init_board(const int rows, const int cols) {
  char **board = malloc((size_t)rows * sizeof(char *));
  for (int i = 0; i < rows; i++) {
    board[i] = (char *)malloc((size_t)(cols + 2) * sizeof(char));
  }

  for (int i = 0; i < cols; i++) {
    board[0][i] = '#';
    board[rows - 1][i] = '#';
  }
  board[0][cols] = '\n';
  board[0][cols + 1] = '\0';
  board[rows - 1][cols] = '\n';
  board[rows - 1][cols + 1] = '\0';

  for (int i = 1; i < rows - 1; i++) {
    board[i][0] = '#';
    for (int j = 1; j < cols - 1; j++) {
      board[i][j] = ' ';
    }
    board[i][cols - 1] = '#';
    board[i][cols] = '\n';
    board[i][cols + 1] = '\0';
  }
  return board;
}

void add_snack_to_board(game_state_t *state) {
  state->board[state->snakes->head_row][state->snakes->head_col] = 'D';
  state->board[state->snakes->tail_row][state->snakes->tail_col] = 'd';
  state->board[state->snakes->tail_row][state->snakes->tail_col + 1] = '>';
}

/* Task 1 */
game_state_t *create_default_state() {
  char **temp_board = init_board(18, 20);
  temp_board[2][9] = '*';

  game_state_t *game_state = malloc(sizeof(game_state_t));
  if (!game_state) malloc_fail();

  game_state->snakes = malloc(sizeof(snake_t));
  if (!game_state->snakes) {
    free(game_state);
    malloc_fail();
  }

  game_state->snakes[0].tail_row = 2;
  game_state->snakes[0].tail_col = 2;
  game_state->snakes[0].head_row = 2;
  game_state->snakes[0].head_col = 4;
  game_state->snakes[0].live = true;

  game_state->num_rows = 18;
  game_state->board = temp_board;
  game_state->num_snakes = 1;

  add_snack_to_board(game_state);

  return game_state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  if (state == NULL) return;

  for (int i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s", state->board[i]);
  }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  char str[] = "wasd";
  if (strchr(str, c) == NULL) {
    return false;
  }
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  char str[] = "WASDx";
  if (strchr(str, c) == NULL) {
    return false;
  }
  return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  char str[] = "wasd^<v>WASDx";
  if (strchr(str, c) == NULL) {
    return false;
  }
  return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  char str[] = "^<v>";
  char str2[] = "wasd";
  if (strchr(str, c) != NULL) {
    return str2[strchr(str, c) - str];
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  char str[] = "WASD";
  char str2[] = "^<v>";
  if (is_head(c)) {
    return str2[strchr(str, c) - str];
  }
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  char str[] = "vsS";
  char str2[] = "^wW";
  if (strchr(str, c) != NULL) {
    return cur_row + 1;
  }
  if (strchr(str2, c) != NULL) {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  char str[] = ">dD";
  char str2[] = "<aA";
  if (strchr(str, c) != NULL) {
    return cur_col + 1;
  }
  if (strchr(str2, c) != NULL) {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  snake_t *snake = &state->snakes[snum];
  unsigned int row = snake->head_row;
  unsigned int col = snake->head_col;
  char head_upper[] = "WASD";
  char head_lower[] = "wasd";
  char *head_pt = strchr(head_upper, get_board_at(state, row, col));
  if (head_pt == NULL) {
    head_pt = strchr(head_lower, get_board_at(state, row, col));
  }
  if (head_pt != NULL) {
    int index = (int)(head_pt - head_upper);
    switch (index) {
      case 0:
        return get_board_at(state, row - 1, col);
      case 1:
        return get_board_at(state, row, col - 1);
      case 2:
        return get_board_at(state, row + 1, col);
      case 3:
        return get_board_at(state, row, col + 1);
      default: ;
    }
  }
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  snake_t *snake = &state->snakes[snum];
  unsigned int row = snake->head_row;
  unsigned int col = snake->head_col;
  char head[] = "WASD";
  char current_head = get_board_at(state, row, col);
  char *head_pt = strchr(head, current_head);
  int index = (int)(head_pt - head);
  state->board[row][col] = head_to_body(current_head);
  switch (index) {
    case 0:
      snake->head_row--;
      state->board[row - 1][col] = current_head;
      return;
    case 1:
      snake->head_col--;
      state->board[row][col - 1] = current_head;
      return;
    case 2:
      snake->head_row++;
      state->board[row + 1][col] = current_head;
      return;
    case 3:
      snake->head_col++;
      state->board[row][col + 1] = current_head;
    default: ;
  }
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  snake_t *snake = &state->snakes[snum];
  unsigned int row = snake->tail_row;
  unsigned int col = snake->tail_col;
  char tail[] = "wasd";
  char current_tail = get_board_at(state, row, col);
  char *tail_pt = strchr(tail, current_tail);
  int index = (int)(tail_pt - tail);
  state->board[row][col] = ' ';
  switch (index) {
    case 0:
      snake->tail_row--;
      state->board[row - 1][col] = body_to_tail(state->board[row - 1][col]);
      return;
    case 1:
      snake->tail_col--;
      state->board[row][col - 1] = body_to_tail(state->board[row][col - 1]);
      return;
    case 2:
      snake->tail_row++;
      state->board[row + 1][col] = body_to_tail(state->board[row + 1][col]);
      return;
    case 3:
      snake->tail_col++;
      state->board[row][col + 1] = body_to_tail(state->board[row][col + 1]);
    default: ;
  }
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  for (int i = 0; i < state->num_snakes; i++) {
    if (state->snakes[i].live) {
      char next_des = next_square(state, (unsigned)i);
      if (is_snake(next_des) || next_des == '#') {
        set_board_at(state, state->snakes[i].head_row,
          state->snakes[i].head_col, 'x');
        state->snakes[i].live = false;
      } else if (next_des == '*') {
        update_head(state, (unsigned)i);
        add_food(state);
      } else {
        update_head(state, (unsigned)i);
        update_tail(state, (unsigned)i);
      }
    }
  }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  if (fp == NULL) return NULL;

  size_t current_size = MAX_LINE;
  char *buffer = malloc(MAX_LINE * sizeof(char));

  if (buffer == NULL) {
    free(buffer);
    return NULL;
  }

  size_t pos = 0;

  int c;
  while ((c = fgetc(fp)) != EOF) {
    if (pos + 1 >= current_size) {
      current_size *= 2;
      buffer = realloc(buffer, current_size * sizeof(char));

      if (buffer == NULL) {
        free(buffer);
        return NULL;
      }
    }

    buffer[pos++] = (char)c;

    if (c == '\n') {
      buffer[pos++] = '\0';
      return buffer;
    }
  }
  if (pos == 0) {
    buffer[0] = '\0';
  }
  return buffer;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  game_state_t *result = malloc(sizeof(game_state_t));
  if (!result) return NULL;

  result->num_snakes = 0;
  result->num_rows = 0;
  result->snakes = NULL;
  result->board = NULL;

  char *read_line_result = read_line(fp);
    while (read_line_result != NULL && read_line_result[0] != '\0') {
      result->num_rows++;

      char **new_board = realloc(result->board,
        result->num_rows * sizeof(char *));

      if (!new_board) {
        result->num_rows--;
        free_game_state_t(result);
        return NULL;
      }

      result->board = new_board;
      result->board[result->num_rows - 1] = read_line_result;
      read_line_result = read_line(fp);
  }
  if (result->board == NULL) {
    free_game_state_t(result);
    return NULL;
  }
  return result;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  unsigned int tail_row = state->snakes[snum].tail_row;
  unsigned int tail_col = state->snakes[snum].tail_col;
  char current_char = get_board_at(state, tail_row, tail_col);

  while (!is_head(current_char)) {
    tail_row = get_next_row(tail_row, current_char);
    tail_col = get_next_col(tail_col, current_char);
    current_char = get_board_at(state, tail_row, tail_col);
  }
  state->snakes[snum].head_row = tail_row;
  state->snakes[snum].head_col = tail_col;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  if (state == NULL) return NULL;

  for (unsigned int row = 0; row < state->num_rows; row++) {
    for (unsigned int col = 0; ;col++) {
      char current_char = get_board_at(state, row, col);
      if (current_char == '\n') break;

      if (is_tail(current_char)) {
        state->snakes = realloc(state->snakes, (state->num_snakes + 1) * sizeof(snake_t));
        if (!state->snakes) {
          free_game_state_t(state);
          return NULL;
        }
        state->snakes[state->num_snakes].tail_row = row;
        state->snakes[state->num_snakes].tail_col = col;
        find_head(state, state->num_snakes);
        state->snakes[state->num_snakes].live = true;
        state->num_snakes++;
      }
    }
  }
  return state;
}
