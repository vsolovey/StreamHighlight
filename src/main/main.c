#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "common.h"
#include "colors.h"
#include "color_settings_parser.h"
#include "highlight.h"


void get_buf(char **buf, size_t *len, int new_len) {
	int new_buf_len = new_len * 21;
	if (new_buf_len > *len) {
		*buf = (char*)realloc(*buf, new_buf_len);
	}
}

/*
   args: { -s substring [-c color] | -w word [-c color] } | -f config_file | -h
-s substring for emphase
-c terminal color
-w search for words only
*/
void main(int argc, char **argv) {
	colors_init();

	char *data[2][DATA_MAX_LEN];
	int data_len = 0;
	int ok = parse_args(argv[0], argv+1, argc-1, data, &data_len);

	if (ok == TRUE) {
		size_t rd_len = 0;
		char *rdbuf = NULL;
        int stdread = getline(&rdbuf, &rd_len, stdin);
		size_t wr_len = 0;
		char *wrbuf = NULL;
		get_buf(&wrbuf, &wr_len, rd_len);
		while (feof(stdin) == 0) {
			int read = strlen(rdbuf);
			int write = emphase_line(rdbuf, read, wrbuf, data, data_len);
			int written = fwrite(wrbuf, 1, write, stdout);
			stdread = getline(&rdbuf, &rd_len, stdin);
			get_buf(&wrbuf, &wr_len, rd_len);
		}
		free(wrbuf);
	}
}
