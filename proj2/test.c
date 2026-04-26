#include <ctype.h>
#include <stdio.h>

int dot(int *input1, int *input2,int numOfElement, int stride1, int stride2) {
    if (numOfElement < 1) {
        return 36;
    }
    if (stride1 < 1 || stride2 < 1) {
        return 37;
    }
    int answer = 0;
    int index1 = 0;
    int index2 = 0;
    for (int i = 0; i < numOfElement; i++) {
        int current1 = input1[index1];
        int current2 = input2[index2];
        answer += current1 * current2;
        index1 += stride1;
        index2 += stride2;
    }
    return answer;
}

int *matmul(int *arr0, int R_arr0, int C_arr0, int *arr1, int R_arr1,
            int C_arr1, int *answer) {
    int num = R_arr0;
    int index = 0;
    for (int i = 0; i < num; i ++) {
        for (int j = 0; j < num; j++) {
            answer[index] = dot(arr0 + i * C_arr0, arr1 + j,
                C_arr0, 1, num);
            index++;
        }
    }
    return answer;
}

int main() {
    int input1[10];
    int input2[10];
    int output[10];

    int current = 1;
    for (int i = 0; i < 10; i++) {
        input1[i] = current;
        input2[i] = current;
        current++;
    }

    matmul(input1, 2, 5,
        input2, 5, 2, output);
    for (int i = 0; i < 4; i++) {
        printf("%d ", output[i]);
    }

    // input1[0] = 4;
    // input2[0] = 5;
    // matmul(input1, 1, 1,
    // input2, 1, 1, output);
    // for (int i = 0; i < 1; i++) {
    //     printf("%d ", output[i]);
    // }

    return 0;
}
