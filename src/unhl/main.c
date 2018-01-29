#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "common.h"

void get_buf(char **buf, size_t *len, int new_len) {
	int new_buf_len = new_len;
	if (new_buf_len > *len) {
		*buf = (char*)realloc(*buf, new_buf_len);
	}
}

int get_candidate(char *input, int len, int pos) {
    while (pos < len && input[pos] != '\e') {
        pos = pos + 1;
    }
    return pos;
}

int parse_esc(char *src, int len, int *pos) {
    int i = *pos;
    int tmp = i;
    if (src[i] == '\e' && src[i + 1] == '[') {
        i = i + 2;
        *pos = i;
    }
    return i > tmp;
}

int parse_param_list(char *src, int len, int *pos) {
    int i = *pos;
    int tmp = i;
    while (src[i] >= '0' && src[i] <= '9' || src[i] == ';') {
        while (i < len && src[i] >= '0' && src[i] <= '9') {
            i = i + 1;
        }
        while (i < len && src[i] == ';') {
            i = i + 1;
        }
    }
    if (i > tmp) {
        *pos = i;
    }
    return *pos == i; // always true
}

int test_candidate(char *input, int len, int pos) {
    int ret;
    int *i = &pos;
    if (parse_esc(input, len, i) && parse_param_list(input, len, i) && input[*i] == 'm') {
        ret = *i + 1;
    } else {
        ret = len-1;
    }
    return ret;
}

void find_sgr(char *input, int len, int *begin, int *end) {
    int beg = *begin, e = len;
    while (beg < len && e == len) {
        beg = get_candidate(input, len, beg);
        e = test_candidate(input, len, beg);
        if (e == len) {
            beg = beg + 1;
        }
    }
    *begin = beg;
    *end = e;
}

void copy(char *from, int from_base, int from_len, char *to, int to_base) {
	char *src = from + from_base;
	char *dst = to + to_base;
	memcpy(dst, src, from_len);
}

int strip_line(char *input, int len, char *output) {
    int i = 0, o = 0;
    int prev = 0, end = 0;
    find_sgr(input, len, &i, &end);
    int sublen = i - prev;
    while (i < len) {
        copy(input, prev, sublen, output, o);
        prev = end; i = end; o = o + sublen;
        find_sgr(input, len, &i, &end);
        sublen = i - prev;
    }
    copy(input, prev, sublen, output, o);
    o = o + sublen;
    return o;
}

void main(int argc, char **argv) {
    size_t rd_len = 0;
    char *rdbuf = NULL;
    int stdread = getline(&rdbuf, &rd_len, stdin);
    size_t wr_len = 0;
    char *wrbuf = NULL;
    get_buf(&wrbuf, &wr_len, rd_len);
    while (feof(stdin) == 0) {
        int read = strlen(rdbuf);
        int write = strip_line(rdbuf, read, wrbuf);
        int written = fwrite(wrbuf, 1, write, stdout);
        stdread = getline(&rdbuf, &rd_len, stdin);
        get_buf(&wrbuf, &wr_len, rd_len);
    }
    free(wrbuf);
}
