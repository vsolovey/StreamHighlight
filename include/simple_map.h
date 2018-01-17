#ifndef SIMPLE_MAP_H_
#define SIMPLE_MAP_H_

int map_contain(char *key);
void map_put(char *key, char *value);
char* map_get(char *key);
int map_size();

#endif
