#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define MAXN 1000000

void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
int findSmallestRepeated(int arr[], int size);

int main(int argc, char *argv[]) {
    FILE *fin;
    int *arr, size, smallestRepeated;
    int num, i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "Can not open \"%s\"\n", argv[1]);
        return EXIT_FAILURE;
    }

    size = 0;
    while (fscanf(fin, "%d", &num) == 1) {
        size++;
    }

    arr = (int *)malloc(size * sizeof(int));

    rewind(fin);
    for (i = 0; i < size; i++) {
        fscanf(fin, "%d", &arr[i]);
    }

    fclose(fin);

    clock_t start_time = clock();

    mergeSort(arr, 0, size - 1);

    smallestRepeated = findSmallestRepeated(arr, size);

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    if (smallestRepeated != -1) {
        printf("%d\n", smallestRepeated);
    } else {
        printf("Nessun elemento ripetuto.\n");
    }

    printf("Tempo di esecuzione: %f secondi\n", elapsed_time);

    free(arr);

    return EXIT_SUCCESS;
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    int m;
    if (l < r) {
        m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int findSmallestRepeated(int arr[], int size) {
    int smallestRepeated = -1;
    int smallestSeen = arr[0];
    int count = 1, i;

    for (i = 1; i < size; i++) {
        if (arr[i] == smallestSeen) {
            count++;
        } else if (count > 1 && (smallestRepeated == -1 || smallestSeen < smallestRepeated)) {
            smallestRepeated = smallestSeen;
        } else if (count == 1 || arr[i] < smallestSeen) {
            smallestSeen = arr[i];
            count = 1;
        }
    }

    return smallestRepeated;
}
