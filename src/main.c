#include <stdio.h>
#include <string.h>
#include "simple_map.h"
#include "common.h"

int BUF_LEN = 256;


char esc = '\x1B';
char *cyan = "\x1B[36m"; // '\x24'
char *decolor = "\x1B[0m";

int TRUE = 1;
int FALSE = 0;

int UNKNOWN = 0;
int SUBSTRING = 1;
int WORD = 2;
int COLOR = 3;
int SETTINGS = 4;
int HELP = 5;

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

/* candidate - это строка, начинающаяся с символа ch
ch - first character of a string
data - array of strings
data_len - length of data
data_pos - start position for looking for candidates
returns index of candidate. If the index equals data_len, then data have no candidates.
*/
int get_candidate(char ch, char *data[], int data_len, int data_pos) {
	while (data_pos < data_len && data[data_pos][0] != ch) {
		data_pos = data_pos + 1;
	}
	return data_pos;
}

int matched(char substring[], char rdbuf[], int pos, int rdbuf_len) {
	char *string = rdbuf + pos;
	int len = rdbuf_len - pos;
	int i = 0;
	while (/*substring[i] != 0 &&*/ i < len && substring[i] == string[i]) {
		i = i + 1;
	}
	return substring[i] == 0;
}

void find_substr(char *data[DATA_MAX_LEN], int data_len, char rdbuf[], int rdbuf_len, int *rdpos_cur, int *data_index) {
	int i = -1;
	int start_from = 0;
	int pos = *rdpos_cur;
	while (pos < rdbuf_len && i == -1) {
		int candidate = get_candidate(rdbuf[pos], data, data_len, start_from);
		if (candidate == data_len) {
			pos = pos + 1;
		} else {
   		while (candidate < data_len && !matched(data[candidate], rdbuf, pos, rdbuf_len)) {
   			start_from = candidate + 1;
   			candidate = get_candidate(rdbuf[pos], data, data_len, start_from);
   		}
			if (candidate < data_len) {
   			i = candidate;
   			*data_index = candidate;
   		} else {
				pos = pos + 1;
				start_from = 0;
   		}
		}
		*rdpos_cur = pos;
	}
}

void copy(char src[], int src_len, char dst_base[], int *dst_offset) {
	char *dst = dst_base + *dst_offset;
	memcpy(dst, src, src_len);
	*dst_offset = *dst_offset + src_len;
}

int emphase_substr(char str[], char *color, int str_len, char wrbuf[], int *pos) {
	copy(color, strlen(color), wrbuf, pos);
	copy(str, str_len, wrbuf, pos);
	copy(decolor, strlen(decolor), wrbuf, pos);
}

int emphase_line(char rdbuf[], int rdbuf_len, char wrbuf[], /*int wrbuflen,*/ char *data[2][DATA_MAX_LEN], int data_len) {
	int rdpos_prev = 0, rdpos_cur = 0;
	int wrpos_cur = 0;
	int data_index = 0;
	find_substr(data[0], data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	while (rdpos_cur < rdbuf_len) {
		char *d = data[0][data_index];
		int d_len = strlen(d);

		char *tmp = rdbuf + rdpos_prev;
		int tmp_len = rdpos_cur - rdpos_prev;
		copy(tmp, tmp_len, wrbuf, &wrpos_cur);
		emphase_substr(d, data[1][data_index], d_len, wrbuf, &wrpos_cur);

		rdpos_cur = rdpos_cur + d_len;
		rdpos_prev = rdpos_cur;

		find_substr(data[0], data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	}
		char *tmp = rdbuf + rdpos_prev;
		int tmp_len = rdpos_cur - rdpos_prev;
		copy(tmp, tmp_len, wrbuf, &wrpos_cur);
	return wrpos_cur;
}

/*
   args: { -s substring [-c color] | -w word [-c color] } | -f config_file | -h
-s substring for emphase
-c terminal color
-w search for words only
*/
void main(int argc, char **argv) {
	char rdbuf[BUF_LEN];
	char wrbuf[BUF_LEN*21];

	char *data[2][DATA_MAX_LEN];
	int data_len = 0;
	int ok = parse_args(argv, argc, data, &data_len);
	char *ch = NULL;

	if (ok == TRUE) {
		fgets(rdbuf, BUF_LEN, stdin);
		while (feof(stdin) == 0) {
			int read = strlen(rdbuf);
			int write = emphase_line(rdbuf, read, wrbuf, data, data_len);
			int written = fwrite(wrbuf, 1, write, stdout);
			fgets(rdbuf, BUF_LEN, stdin);
		}
	}
}
