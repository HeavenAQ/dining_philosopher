#include "philosopher.h"
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 5

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// functions
void dine(Philosopher philosophers[N]);
void start_dining(Philosopher *philosopher);
void print_all_statuses();
void print_status(Philosopher *philosopher);
static inline void all_seated(Philosopher *philosopher);
static inline void leaving();

// global variables
int start = true;
pthread_mutex_t sticks[N];
Philosopher philosophers[N];

int main(void)
{
    for (int i = 0; i < N; ++i) {
        philosophers[i].id = i;
        philosophers[i].left_stick = i;
        philosophers[i].right_stick = (i + 1) % N;
        philosophers[i].status = THINKING;
        pthread_mutex_init(&sticks[i], 0);
    }

    struct sigaction sa;
    sa.sa_handler = leaving;
    sigaction(SIGINT, &sa, NULL);

    {
        // Welcome Messages
        puts(ANSI_COLOR_CYAN "Dining Philosophers Problem");
        puts("===========================");
        puts("The table is empty." ANSI_COLOR_RESET);

        // Start
        dine(philosophers);

        // print out finished message
        puts(ANSI_COLOR_CYAN "The table is empty.");
    }
}

static inline void leaving() { start = false; }

void dine(Philosopher philosophers[N])
{
    pthread_t threads[N];
    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], NULL, (void *)all_seated, &philosophers[i]);
    }
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], NULL, (void *)start_dining,
                       &philosophers[i]);
    }
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }
}

void print_status(Philosopher *philosopher)
{
    switch (philosopher->status) {
    case THINKING:
        printf("T ");
        break;
    case HUNGRY:
        printf(ANSI_COLOR_YELLOW "H " ANSI_COLOR_RESET);
        break;
    case EATING:
        printf(ANSI_COLOR_BLUE "E " ANSI_COLOR_RESET);
        break;
    case LEFT:
        printf(ANSI_COLOR_RED "L " ANSI_COLOR_RESET);
        break;
    case SEATED:
        printf(ANSI_COLOR_GREEN "S " ANSI_COLOR_RESET);
        break;
    }
}

void print_all_statuses()
{
    printf("Current State:\n\t");
    for (int i = 0; i < N; ++i) {
        printf("%d ", philosophers[i].id);
    }
    printf("\n\t");

    for (int i = 0; i < N; ++i) {
        print_status(&philosophers[i]);
    }
    printf("\n\n");
}

static inline void all_seated(Philosopher *philosopher)
{
    philosopher->status = SEATED;
    print_all_statuses();
}

void start_dining(Philosopher *philosopher)
{
    while (start) {
        if (philosopher->id == N - 1) {
            pthread_mutex_lock(&sticks[philosopher->right_stick]);
            philosopher->status = HUNGRY;
            print_all_statuses();
            pthread_mutex_lock(&sticks[philosopher->left_stick]);
        } else {
            pthread_mutex_lock(&sticks[philosopher->left_stick]);
            philosopher->status = HUNGRY;
            print_all_statuses();
            pthread_mutex_lock(&sticks[philosopher->right_stick]);
        }

        philosopher->status = EATING;
        print_all_statuses();
        sleep(EATTIME);

        pthread_mutex_unlock(&sticks[philosopher->left_stick]);
        pthread_mutex_unlock(&sticks[philosopher->right_stick]);
        philosopher->status = THINKING;
        print_all_statuses();
        sleep(THINKTIME);
    }

    printf(ANSI_COLOR_MAGENTA "Philosopher %d left.", philosopher->id);
    puts(ANSI_COLOR_RESET "");
    philosopher->status = LEFT;
    print_all_statuses();
}
