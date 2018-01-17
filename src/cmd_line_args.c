#include <stdio.h>
#include <string.h>
#include "common.h"
#include "color.h"
#include "simple_map.h"

#define UNKNOWN 0
#define SUBSTRING 1
#define WORD 2
#define COLOR 3
#define SETTINGS 4
#define HELP 5

void print_usage(char *cmd) {
	printf("usage: %s { -s substring [-c color] | -w word [-c color] } | -f config_file | -h\n    -s substring   look for substring\n    -w word        look for word\n    -c color       paint substring with color\n    -f filename    use config file\n    -h             print help\n", cmd);
}

int parse_arg_type(char *arg) {
	int arg_type = UNKNOWN;
	if (arg[0] == '-' && arg[1] != 0 && arg[2] == 0) {
		switch(arg[1]) {
			case('s'):
				arg_type = SUBSTRING;
				break;
			case('w'):
				arg_type = WORD;
				break;
			case('c'):
				arg_type = COLOR;
				break;
			case('f'):
				arg_type = SETTINGS;
				break;
			case('h'):
				arg_type = HELP;
				break;
		}
	}
	
	return arg_type;
}

int append_substring(char **argv, int argc, int arg_pos, char *strings[], int string_pos) {
	int ret = TRUE;
	if (arg_pos < argc) {
		strings[string_pos] = argv[arg_pos];
	} else {
		print_usage(argv[0]);
		ret = FALSE;
	}
	return ret;
}

int is_valid_color(char *color) {
	int len = strlen(color);
	int i = 0;
	if (len <= 3 && len > 0) {
		char ch = color[i];
		while (i < len && ch >= '0' && ch <= '9') {
			i = i + 1;
			ch = color[i];
		}
	}
	return i == len;
}

int append_color(char **argv, int argc, int arg_pos, char *colors[], int color_pos) {
	int ret = TRUE;
	if (color_pos >= 0 && arg_pos < argc) {
		char *color = argv[arg_pos];
		if (is_valid_color(color)) {
			if (!map_contain(color)) {
				sprintf(map_put(color), "\x1B[%sm", color);
			}
		} else {
			sprintf(map_put(color), "%s", cyan);
		}
		colors[color_pos] = map_get(color);
	} else {
		print_usage(argv[0]);
		ret = FALSE;
	}
	return ret;
}

int parse_args(char **argv, int argc, char *data[2][DATA_MAX_LEN], int *data_len) {
	int ok = TRUE;
	int pos = *data_len;
	int arg = 1;
	while (arg < argc && ok == TRUE) {
		int type = parse_arg_type(argv[arg]);
		if (type == UNKNOWN) {
			printf("unknown arg: %s\n", argv[arg]);
			print_usage(argv[0]);
			ok = FALSE;
		} else if (type == SUBSTRING) {
			arg = arg + 1;
			ok = append_substring(argv, argc, arg, data[0], pos);
			data[1][pos] = cyan;
			pos = pos + 1;
		} else if (type == COLOR) {
			arg = arg + 1;
			ok = append_color(argv, argc, arg, data[1], pos - 1);
		} else if (type == HELP) {
			print_usage(argv[0]);
			ok = FALSE;
		}
		arg = arg + 1;
	}
	*data_len = pos;
	return ok;
}
