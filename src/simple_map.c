#include <malloc.h>



const int VALUE_SIZE = 10;

char **keys;
char **values;

int max;
int len = 0;
int in_use = 0;

int map_init(int max_map_size) {
	int initialized = in_use != 0;
	if (!initialized) {
		max = max_map_size;
		keys = (char**)malloc(max * sizeof(int));
		values = (char**)malloc(max * VALUE_SIZE);
		in_use = 1;
	}
	return !initialized;
}

void map_dispose() {
	free(keys);
	free(values);
	in_use = 0;
}

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
	char *ret = NULL;
	int i = map_get_key_index(key);
	if (i < len) {
		ret = values[i];
	}
	return ret;
}

char* map_put(char *key) {
	char *value = NULL;
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
