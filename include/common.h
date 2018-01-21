#ifndef COMMON_H_
#define COMMON_H_

#define DATA_MAX_LEN 256
#define COLOR_SIZE 10

#define TRUE 1
#define FALSE 0

#define UNKNOWN 0
#define SUBSTRING 1
#define WORD 2
#define COLOR 3
#define SETTINGS 4
#define HELP 5

struct elem {
    short type;
    char *str;
    char *color;
};

typedef struct elem elem;

#endif
