#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H
#define EATTIME 1
#define THINKTIME 3

enum Status { THINKING, HUNGRY, EATING, LEFT, SEATED };
typedef struct Philosopher {
    int id;
    short left_stick;
    short right_stick;
    enum Status status;
} Philosopher;

#endif
