#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../common/methods.h"

#define ROWS 140
#define COLS 140

#define BUFFER_SIZE 5

static const int EMPTY = -10;
static const int SYMBOL = 10;

int* map_to_row(const char *line) {
    int *row = (int*) malloc(COLS * sizeof(int));
    for (int i = 0; i < COLS; ++i) {
        char c = line[i];
        if (isdigit(c))
            row[i] = c - '0';
        else if (c == '.')
            row[i] = EMPTY;
        else
            row[i] = SYMBOL;
    }
    return row;
}

int** read_lines(FILE *file) {
    int i = 0;
    char line[COLS+2];
    int **array = (int**) malloc(ROWS * sizeof(int*));

    while (fgets(line, sizeof(line), file) != NULL) {
        array[i++] = map_to_row(line);
    }
    return array;
}

void check_single(int **array, int current, int next_i, int next_j, int *found) {
    if (*found == 1)
        return;
    if (next_i < 0 || next_j < 0 || next_i > ROWS-1 || next_j > COLS-1)
        return;
    int next = array[next_i][next_j];
    // skip neighbor numbers
    if (next != EMPTY && next != SYMBOL)
        return;
    *found = next - current > 0? 1 : 0;
}

void test_neighbor(int **array, int i, int j, int *has_neighbor) {
    int current = array[i][j];
    check_single(array, current, i-1, j-1, has_neighbor);
    check_single(array, current, i-1, j, has_neighbor);
    check_single(array, current, i-1, j+1, has_neighbor);

    check_single(array, current, i, j-1, has_neighbor);
    check_single(array, current, i, j+1, has_neighbor);

    check_single(array, current, i+1, j-1, has_neighbor);
    check_single(array, current, i+1, j, has_neighbor);
    check_single(array, current, i+1, j+1, has_neighbor);
}

int main() {
    FILE *file = fopen("2023/day_3/input.txt", "r");
    if (file == NULL) {
        perror("error opening file");
        return 1;
    }

    int **array = read_lines(file);

    int total = 0;
    for (int i = 0; i < ROWS; ++i) {

        // number metadata
        int ind = 0;
        char buff[BUFFER_SIZE] = {0};
        int has_neighbor = 0;

        for (int j = 0; j < COLS; ++j) {

            int c = array[i][j];
            // skip & restart if not relevant
            if (c == EMPTY || c == SYMBOL) {
                // before reset, check last number
                if (has_neighbor == 1)
                    total += atoi(buff);

                ind = 0;
                bzero(buff, BUFFER_SIZE);
                has_neighbor = 0;

                continue;
            }

            buff[ind++] = c + '0';

            test_neighbor(array, i, j, &has_neighbor);
        }

        if (has_neighbor == 1)
            total += atoi(buff);
    }

    free_int_2d_matrix(array, ROWS);

    printf("Result: %d\n", total);
    fclose(file);

    return 0;
}