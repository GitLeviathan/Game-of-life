#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80
#define SQUARE HEIGHT *WIDTH

void create_empty_array(int ***);
void input(int **);
int set_speed();
void print_field(int **);
void chech_ded_alive(int **, int **);
int swap_arrays(int **, int **);
int check_end(int **);

int main() {
    int **field_before = NULL, **field_after = NULL, speed = 1000000, identical_fields = 0;
    FILE *temp_garbage;
    create_empty_array(&field_before);
    create_empty_array(&field_after);
    input(field_before);
    temp_garbage = freopen("/dev/tty", "r", stdin);
    speed = set_speed();
    do {
        print_field(field_before);
        chech_ded_alive(field_before, field_after);
        identical_fields = swap_arrays(field_before, field_after);
        usleep(speed);
    } while (check_end(field_before) != 0 && identical_fields);
    print_field(field_before);
    fclose(temp_garbage);
    free(field_before);
    free(field_after);
    return 0;
}

void fill_array_0(int **field) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
        }
    }
}

void create_empty_array(int ***field) {
    *field = malloc(HEIGHT * sizeof(int *) + HEIGHT * WIDTH * sizeof(int));
    int *tmp = (int *)(*field + HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        (*field)[i] = tmp + i * WIDTH;
    }
    fill_array_0(*field);
}

void input(int **field) {
    int *temp_array, temp_x = -2, temp_i = 0;
    temp_array = malloc(SQUARE * sizeof(int));
    while (temp_x != -1) {
        scanf("%d", &temp_x);
        if (temp_x != -1) {
            temp_array[temp_i] = temp_x;
            temp_i++;
        }
    }
    for (int i = 0; i < temp_i; i += 2) {
        int j = i + 1;
        field[temp_array[i]][temp_array[j]] = 1;
    }
    free(temp_array);
}

int set_speed() {
    float speed;
    printf("Please set the speed (enter frame rate per second)\n");
    scanf("%f", &speed);
    speed = 1000000 / speed;
    return (int)speed;
}

void print_field(int **field) {
    printf("\033c");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
    fflush(stdout);
}

int couting_neighbours(int **field_before, int i, int j) {
    int count = 0;
    for (int y = i - 1; y <= i + 1; y++) {
        for (int x = j - 1; x <= j + 1; x++) {
            count += field_before[(y + HEIGHT - 1) % (HEIGHT - 1)][(x + WIDTH - 1) % (WIDTH - 1)];
        }
    }
    count -= field_before[i][j];
    return count;
}

void chech_ded_alive(int **field_before, int **field_after) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int count = couting_neighbours(field_before, i, j);
            switch (count) {
                case 2:
                    field_after[i][j] = field_before[i][j];
                    break;
                case 3:
                    field_after[i][j] = 1;
                    break;
                default:
                    field_after[i][j] = 0;
                    break;
            }
        }
    }
}

int swap_arrays(int **field_before, int **field_after) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field_before[i][j] != field_after[i][j]) flag = 1;
            field_before[i][j] = field_after[i][j];
        }
    }
    return flag;
}

int check_end(int **field) {
    int total_count = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) total_count++;
        }
    }
    return total_count;
}
