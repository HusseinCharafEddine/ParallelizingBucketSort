#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBERS 100000
#define MAX_SIZE 1000000

int min, max, n, k;
int **numbers = NULL;
int *idxcounter = NULL;
int *nums = NULL;
int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void bucket_sort(int id)
{
    int start = 0;
    for (int i = 0; i < id; i++)
    {
        start += idxcounter[i];
    }
    int end = start + idxcounter[id];

    qsort(numbers[id], idxcounter[id], sizeof(int), cmpfunc);
    for (int i = start; i < end; i++)
    {
        nums[i] = numbers[id][i - start];
    }
}

int main()
{
    double total_time = 0.0;

    for (int trial = 0; trial < 10; trial++)
    {

        clock_t start_time = clock();
        n = 10000;

        k = 100;
        srand(time(NULL));
        min = 0;
        max = 100000;

        numbers = (int **)malloc(k * sizeof(int *));
        if (numbers == NULL)
        {
            printf("Memory allocation failed. Exiting...\n");
            return 1;
        }

        for (int i = 0; i < k; i++)
        {
            numbers[i] = (int *)malloc(MAX_SIZE * sizeof(int));
            if (numbers[i] == NULL)
            {
                printf("Memory allocation failed. Exiting...\n");
                for (int j = 0; j < i; j++)
                {
                    free(numbers[j]);
                }
                free(numbers);
                return 1;
            }
        }

        idxcounter = (int *)calloc(k, sizeof(int));
        if (idxcounter == NULL)
        {
            printf("Memory allocation failed. Exiting...\n");
            for (int i = 0; i < k; i++)
            {
                free(numbers[i]);
            }
            free(numbers);
            return 1;
        }

        nums = (int *)malloc(n * sizeof(int));
        if (nums == NULL)
        {
            printf("Memory allocation failed. Exiting...\n");
            for (int i = 0; i < k; i++)
            {
                free(numbers[i]);
            }
            free(numbers);
            free(idxcounter);
            return 1;
        }

        FILE *file;
        file = fopen("array.txt", "r");
        if (file == NULL)
        {
            printf("Error opening file.\n");
            return 1;
        }

        int num_elements = 0;
        while (fscanf(file, "%d,", &nums[num_elements]) == 1)
        {
            num_elements++;
            if (num_elements >= MAX_SIZE)
            {
                printf("Array size exceeded.\n");
                break;
            }
        }
        fclose(file);
        for (int i = 0; i < n; i++)
        {
            int x = nums[i];
            int index = x / (max / k);
            if (idxcounter[index] >= (MAX_SIZE / k))
            {
                printf("Array size exceeded.\n");
                return 1;
            }
            numbers[index][idxcounter[index]++] = x;
        }

        for (int i = 0; i < k; i++)
        {
            bucket_sort(i);
        }

        clock_t end_time = clock();
        double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        total_time += time_taken;

        free(nums);
        for (int i = 0; i < k; i++)
        {
            free(numbers[i]);
        }
        free(numbers);
        free(idxcounter);
    }

    printf("Average time taken over 10 trials: %f seconds\n", total_time * 100);

    return 0;
}
