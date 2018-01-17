#include <stdio.h>
#include <string.h>
#include "common.h"
#include "colors.h"
#include "cmd_line_args.h"
#include "highlight.h"

int BUF_LEN = 256;

/*
   args: { -s substring [-c color] | -w word [-c color] } | -f config_file | -h
-s substring for emphase
-c terminal color
-w search for words only
*/
void main(int argc, char **argv) {
	colors_init();
	char rdbuf[BUF_LEN];
	char wrbuf[BUF_LEN*21];

	char *data[2][DATA_MAX_LEN];
	int data_len = 0;
	int ok = parse_args(argv[0], argv+1, argc-1, data, &data_len);

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
