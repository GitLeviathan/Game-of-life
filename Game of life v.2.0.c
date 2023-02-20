#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define N 25
#define M 80

#define WHITE "\033[1;33m"
#define RESET "\033[0m"

int count_cells(int **screen, int i, int j);
void count_next_frame(int **screen, int **next_frame);
void input(int **screen);
void output(int **screen);
void make_field();
void clear_screen(int **screen);

int main() {
    char running;
    int change = 10;
    int **screen, **next_frame, frames = 100;
    screen = (int **)calloc(N, sizeof(int *));
    next_frame = (int **)calloc(N, sizeof(int *));
    for (int i = 0; i < N; ++i) {
        screen[i] = (int *)calloc(M, sizeof(int));
        next_frame[i] = (int *)calloc(M, sizeof(int));
    }

    printf("Enter frame rate, recomended 100.\n");
    scanf("%d", &frames);

    input(screen);
    initscr();
    start_color();
    noecho();
    curs_set(0);

    running = 1;
    timeout(frames);

    while (running) {
        timeout(frames);
        char key = getch();

        if (key == 'q') {
            running = 0;
        }
        if (key == '-' && frames < 100000) {
            frames += change;
        }
        if (key == '=' && frames > 10) {
            frames -= change;
        }

        clear();
        make_field();
        output(screen);
        refresh();

        count_next_frame(screen, next_frame);
    }

    echo();
    endwin();
    for (int i = 0; i < N; ++i) {
        free(screen[i]);
        free(next_frame[i]);
    }
    free(screen);
    free(next_frame);
    return 0;
}

void input(int **screen) {
    int how_many_cells;
    printf("How many cells to create? Insert 0 to generate random.\n");
    scanf("%d", &how_many_cells);
    if (how_many_cells == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                screen[i][j] = rand() % 2;
            }
        }
    } else {
        printf("Enter X and Y cells coordinates:\n");
        for (int i = 0; i < how_many_cells; i++) {
            int x, y;
            scanf("%d %d", &x, &y);
            screen[x][y] = 1;
        }
    }
    if (freopen("/dev/tty", "r", stdin) == NULL) printf("ERROR");
}

void output(int **screen) {
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    printw("                Press:  Q to quit    Press:  - / + to change speed");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            move(i + 1, j + 1);
            if (screen[i][j]) {
                attron(COLOR_PAIR(1));
                printw(" ");
                attrset(A_NORMAL);
            }
        }
    }
}

int count_cells(int **screen, int i, int j) {
    int res = 0;
    for (int k = -1; k <= 1; ++k) {
        for (int l = -1; l <= 1; ++l) {
            int new_i = i + k;
            int new_j = j + l;
            if (k != 0 || l != 0) {
                if (new_i == -1) {
                    new_i = N - 1;
                } else if (new_i == N) {
                    new_i = 0;
                }
                if (new_j == -1) {
                    new_j = M - 1;
                } else if (new_j == M) {
                    new_j = 0;
                }
                res += screen[new_i][new_j];
            }
        }
    }
    return res;
}

void count_next_frame(int **screen, int **next_frame) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int env = count_cells(screen, i, j);

            if (screen[i][j] == 0) {
                if (env == 3) {
                    next_frame[i][j] = 1;
                } else {
                    next_frame[i][j] = 0;
                }
            } else if (screen[i][j] == 1) {
                if (env == 2 || env == 3) {
                    next_frame[i][j] = 1;
                } else {
                    next_frame[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            screen[i][j] = next_frame[i][j];
        }
    }
    clear_screen(next_frame);
}

void make_field() {
    printw(" ");
    for (int i = 0; i < M; i++) printw("-");
    printw("\n");
    for (int i = 0; i < N; i++) {
        printw("|                                                                                |\n");
    }
    printw(" ");
    for (int i = 0; i < M; i++) printw("-");
    printw("\n");
}

void clear_screen(int **screen) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            screen[i][j] = 0;
        }
    }
}
