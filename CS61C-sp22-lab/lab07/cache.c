int array[256] = {0};

void accessWords(int array_size, int step_size, int rep_count, int option) {
    for (int k = 0; k < rep_count; k++) {
        for (int index = 0; index < array_size; index += step_size) {
            if (option == 0)
                array[index] = 0; // Option 0: One cache access - write
            else
                array[index] = array[index] + 1; // Option 1: Two cache accesses - read AND write
        }
    }
}

int main() {
    int array_size = 256;
    int step_size = 2;
    int rep_count = 1;
    int option = 1;
    accessWords(array_size, step_size, rep_count, option);

    return 0;
}
