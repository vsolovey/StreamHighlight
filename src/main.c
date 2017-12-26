#include <stdio.h>
#include <string.h>

int LEN = 256;
int DATA_MAX_LEN = 256;
char esc = '\x1B';
char *cyan = "\x1B[36m"; // '\x24'
char *decolor = "\x1B[0m";

int TRUE = -1;
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
	if (arg[0] == '-' && arg[1] != '\0' && arg[2] == '\0') {
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

int append_substring(char **argv, int argc, int arg_pos, char *data[], int data_pos) {
	int ret = TRUE;
	if (arg_pos < argc) {
		data[data_pos] = argv[arg_pos];
	} else {
		print_usage(argv[0]);
		ret = FALSE;
	}
	return ret;
}

int parse_args(char **argv, int argc, char *data[], int *data_len) {
	int ok = TRUE;
	int pos = *data_len;
	int i = 1;
	while (i < argc && ok == TRUE) {
		int type = parse_arg_type(argv[i]);
		if (type == UNKNOWN) {
			printf("unknown arg: %s\n", argv[i]);
			print_usage(argv[0]);
			ok = FALSE;
		} else if (type == SUBSTRING) {
			i = i + 1;
			ok = append_substring(argv, argc, i, data, pos);
			pos = pos + 1;
		} else if (type == HELP) {
			print_usage(argv[0]);
			ok = FALSE;
		}
		i = i + 1;
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

void find_substr(char *data[], int data_len, char rdbuf[], int rdbuf_len, int *rdpos_cur, int *data_index) {
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

int emphase_substr(char str[], int str_len, char wrbuf[], int *pos) {
	copy(cyan, strlen(cyan), wrbuf, pos);
	copy(str, str_len, wrbuf, pos);
	copy(decolor, strlen(decolor), wrbuf, pos);
}

int emphase_line(char rdbuf[], int rdbuf_len, char wrbuf[], /*int wrbuflen,*/ char *data[], int data_len) {
	int rdpos_prev = 0, rdpos_cur = 0;
	int wrpos_cur = 0;
	int data_index = 0;
	find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	while (rdpos_cur < rdbuf_len) {
		char *d = data[data_index];
		int d_len = strlen(d);

		char *tmp = rdbuf + rdpos_prev;
		int tmp_len = rdpos_cur - rdpos_prev;
		copy(tmp, tmp_len, wrbuf, &wrpos_cur);
		emphase_substr(d, d_len, wrbuf, &wrpos_cur);

		rdpos_cur = rdpos_cur + d_len;
		rdpos_prev = rdpos_cur;

		find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
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
	char rdbuf[LEN];
	char wrbuf[LEN*11];

	char *data[DATA_MAX_LEN];
	int data_len = 0;
	int ok = parse_args(argv, argc, data, &data_len);

	if (ok == TRUE) {
		int i = 0;
		while (i < data_len) {
			i = i + 1;
		}

		fgets(rdbuf, LEN, stdin);
      while (feof(stdin) == 0) {
   		int read = strlen(rdbuf);
   		int write = emphase_line(rdbuf, read, wrbuf, data, data_len);
   		int written = fwrite(wrbuf, 1, write, stdout);
			fgets(rdbuf, LEN, stdin);
   	}
	}
}
