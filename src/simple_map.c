#include "common.h"

char *keys[DATA_MAX_LEN];
char values[DATA_MAX_LEN][COLOR_SIZE];

int len = 0;
int in_use = 0;

int map_get_key_index(char *key) {
	int i = 0;
	while (i < len && keys[i] != key) {
		i = i + 1;
	}
	return i;
}

int map_contain(char *key) {
	return map_get_key_index(key) < len;
}

char* map_get(char *key) {
	char *ret = 0;
	int i = map_get_key_index(key);
	if (i < len) {
		ret = values[i];
	}
	return ret;
}

char* map_put(char *key) {
	char *value = 0;
	if (!map_contain(key)) {
		keys[len] = key;
		value = values[len];
		len = len + 1;
	} else {
		value = map_get(key);
	}
	return value;
}

int map_size() {
	return len;
}
