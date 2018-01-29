#include <stdio.h>
#include "common.h"
#include "colors.h"
#include "simple_map.h"
#include "preset_file_parser.h"

void print_usage(char *cmd) {
	printf("usage: %s { -s substring [-c color] | -w word [-c color] }\n       %s -p preset_file\n       %s -h\n    -s substring   look for substring\n    -w word        look for word\n    -c color       set color for substring\n                   color can be:\n                   predefined: [30..37,90..97] - foreground, [40..47,100..107] - background\n                   alias: cyan, light_green, read_bg etc.\n                   custom: 96,64,32 - RGB\n    -f filename    use config file\n    -h             print help\n", cmd, cmd, cmd);
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
			case('p'):
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

int append_color(char **argv, int argc, int arg_pos, elem *e) {
	int ret = TRUE;
	if (arg_pos < argc) {
		char *color = argv[arg_pos];
		if (colors_is_valid(color)) {
			if (!map_contain(color)) {
				map_put(color, colors_get(color));
			}
		} else {
			map_put(color, colors_get_default());
		}
		e->color = map_get(color);
	} else {
		print_usage(argv[0]);
		ret = FALSE;
	}
	return ret;
}

int parse_args(char *cmdname, char **argv, int argc, elem data[DATA_MAX_LEN], int *data_len) {
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
			data[pos].color = colors_get_default();
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
	}
	return ok;
}
