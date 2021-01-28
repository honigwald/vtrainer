#ifndef TRAINER_H
#define TRAINER_H

#define DBFILE "./data/lecture1.db"
#define TRUE 1
#define FALSE 0

#define LEAVE 0
#define TRAINING 1
#define ADD_VOC 2
#define PRINT_VOC 3

#define BEGINNER 0
#define ADVANCED 1
#define LEARNED 2

struct list {
    char *original;
    char *translation;
    int progress;
    struct list *next;
};

typedef struct list list;

/* function declarations */
int menue();
list *add_vocabulary(list *head);
void print_vocabulary(list *head);
void trainer(list *head);
list *initialize();
int save(list *head);
void clear_screen(int line, int column);

#endif
