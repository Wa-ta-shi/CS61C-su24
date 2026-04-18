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

int main() {
    int input1[] = {-3, 4, 4};
    int input2[] = {-3, 4, 4};
    int pt = dot(input1, input2, 3, 1, 1);
    printf("%d\n", pt);

    int result = 0;
    int temp[] = {2, 40, 429, 11, 10, 12, 2, 106, 7};
    for (int i = 0; i < 9; i++) {
        result += temp[i] * temp[i];
    }
    printf("%d", result);

    return 0;
}
