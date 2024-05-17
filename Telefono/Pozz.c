#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXN 1000000

int get_index_from_number(int number)
{
    if (number <= 399999)
    {
        return number % 300000;
    }
    else
    {
        return number % 400000;
    }
}

int get_number_from_index(int index)
{
    if (index <= 99999)
    {
        return index + 300000;
    }
    else
    {
        return index + 400000;
    }
}

int main(int argc, char *argv[])
{
    FILE *fin;
    int num_tel;
    int *A = calloc(MAXN, sizeof(int));
    int i;
    int found = 0;

    if (A == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s input_file_name\n", argv[0]);
        free(A);
        return EXIT_FAILURE;
    }

    fin = fopen(argv[1], "r");
    if (fin == NULL)
    {
        fprintf(stderr, "Can not open \"%s\"\n", argv[1]);
        free(A);
        return EXIT_FAILURE;
    }

    clock_t start_time = clock();

    while (fscanf(fin, "%d", &num_tel) == 1)
    {
        A[get_index_from_number(num_tel)]++;
    }

    fclose(fin);

    for (i = 0; i < MAXN; i++)
    {
        if (A[i] >= 2)
        {
            printf("Numero minimo ripetuto più volte: %d\n", get_number_from_index(i));
            found = 1;
            break;
        }
    }

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Tempo di esecuzione: %f secondi\n", elapsed_time);

    if (!found)
    {
        printf("Nessun numero ripetuto trovato nel file di input.\n");
    }

    free(A);
    return EXIT_SUCCESS;
}
