#ifndef SIMPLE_MAP_H_
#define SIMPLE_MAP_H_

int map_init(int max_map_size);
void map_dispose();

int map_contain(char *key);
char* map_put(char *key);
char* map_get(char *key);
int map_size();

#endif
