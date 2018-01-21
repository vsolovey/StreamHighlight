#include <stdio.h>
#include <string.h>
#include "common.h"
#include "colors.h"
#include "simple_map.h"
#include "preset_file_parser.h"

void print_usage(char *cmd) {
	printf("usage: %s { -s substring [-c color] | -w word [-c color] }\n       %s -f config_file\n       %s -h\n    -s substring   look for substring\n    -w word        look for word\n    -c color       paint substring with color\n    -f filename    use config file\n    -h             print help\n", cmd, cmd, cmd);
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

int append_substring(char **argv, int argc, int arg_pos, short type, elem *e) {
	int ret = TRUE;
	if (arg_pos < argc) {
		e->type = type;
		e->str = argv[arg_pos];
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

int append_color(char **argv, int argc, int arg_pos, elem *e) {
	int ret = TRUE;
	if (arg_pos < argc) {
		char *color = argv[arg_pos];
		if (is_valid_color(color)) {
			if (!map_contain(color)) {
				map_put(color, get_color(color));
			}
		} else {
			map_put(color, get_color(color));
		}
		e->color = map_get(color);
	} else {
		print_usage(argv[0]);
		ret = FALSE;
	}
	return ret;
}

/*void print_arr(int argc, char **argv) {
	int i = 0;
	while (i < argc) {
		printf("%s\n", argv[i]);
		i = i + 1;
	}
}*/

int parse_args(char *cmdname, char **argv, int argc, elem data[DATA_MAX_LEN], int *data_len) {
	//print_arr(argc, argv);
	int ok = TRUE;
	int pos = *data_len;
	short type = UNKNOWN;
	int arg = 0;
	while (arg < argc && ok == TRUE) {
		type = parse_arg_type(argv[arg]);
		if (type == UNKNOWN) {
			printf("unknown arg: %s\n", argv[arg]);
			print_usage(argv[0]);
			ok = FALSE;
		} else if (type == SUBSTRING || type == WORD) {
			arg = arg + 1;
			ok = append_substring(argv, argc, arg, type, &data[pos]);
			data[pos].color = cyan;
			pos = pos + 1;
		} else if (type == COLOR) {
			arg = arg + 1;
			if (pos > 0) {
				ok = append_color(argv, argc, arg, &data[pos - 1]);
			} else {
				print_usage(argv[0]);
				ok = FALSE;
			}
		} else if (type == SETTINGS) {
			arg = arg + 1;
			ok = argc - 1 == arg;
			if (ok) {
				int file_argc = 0;
				char *file_argv[DATA_MAX_LEN];
				parse_file(argv[arg], &file_argc, file_argv);
				ok = validate(file_argc, file_argv);
				if (ok) {
					ok = parse_args(cmdname, file_argv, file_argc, data, data_len);
				}
			}
		} else if (type == HELP) {
			print_usage(cmdname);
			ok = FALSE;
		}
		arg = arg + 1;
	}
	if (type != SETTINGS) {
		*data_len = pos;
		/*int p = 0;
		while (p < data_len) {
			printf("%i: {%i, %s, %s%s}\n", p, (int)data[p].type, data[p].color, data[p].str, decolor);
			p = p + 1;
		}*/
	}
	return ok;
}
