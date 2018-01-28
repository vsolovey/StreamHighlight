#ifndef COLOR_H_
#define COLOR_H_

extern char *decolor;

void colors_init();
void colors_dispose();

char *colors_get_default();
char *colors_get(char *num);

int colors_is_valid(char *color);

#endif