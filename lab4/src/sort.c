#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int slow_sort(int *arr, const int n) {
    int interval, k, i, j, min, c;
    c = 0, k = 2; // (1)
    interval = n / k; // (2)

    while (interval >= 1) { // (3)
        i = 0; // (4)
        while (i < n - interval) { // (5)
            min = i; // (6)
            j = i + interval; // (7)
            while (j < n) { // (8)
                c++; // (9)
                if (arr[j] < arr[min]) // (10)
                    min = j; // (11)
                j = j + interval; // (12)
            }
            int tmp = arr[min]; // (13)
            arr[min] = arr[i]; // (14)
            arr[i] = tmp; // (15)
            i = i + interval; // (16)
        }
        k++; // (17)
        interval = n / k; // (18)
    }
    return c;
}

int main(const int argc, const char *argv[]) {
    srand(0);

    int len = atoi(argv[1]);

    int *arr = malloc(sizeof(int) * len);

    printf("Enter %d elements of array:\n", len);


    for (__auto_type i = 0; i < len; i++) {
        scanf("%d", &arr[i]);
    }


    slow_sort(arr, len);

    printf("Sorted array is:\n");

    for (int i = 0; i < len; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;

}
