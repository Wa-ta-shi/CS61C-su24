#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix
  int32_t b_rows = b_matrix->rows;
  int32_t b_cols = b_matrix->cols;
  for (int i = 0; i < b_rows / 2; i++) {
    for (int j = 0; j < b_cols / 2; j++) {
      uint32_t temp = b_matrix->data[i * b_cols + j];
      b_matrix->data[i * b_cols + j] = b_matrix->data[(b_rows - i) * b_cols + (b_cols - j)];
      b_matrix->data[(b_rows - i) * b_cols + (b_cols - j)] = temp;
    }
  }

  int32_t a_rows = a_matrix->rows;
  int32_t a_cols = a_matrix->cols;

  matrix_t *answer = malloc(sizeof(matrix_t));
  output_matrix = &answer;
  if (!answer) return -1;

  answer->data = malloc((a_rows - b_rows) * (a_cols - b_cols) * sizeof(int32_t));

  for (int i = 0; i < a_rows - b_rows; i++) {
    for (int j = 0; j < a_cols - b_cols; j++) {
      uint32_t temp = 0;
      for (int k = 0; k < b_rows; k++) {
        for (int l = 0 ; l < b_cols; l++) {
          temp += b_matrix->data[k * b_cols + l] *
            a_matrix->data[(i + k) * a_cols + j + l];
        }
      }
      answer->data[i * a_cols + j] = temp;
    }
  }

  return 0;
}

// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
