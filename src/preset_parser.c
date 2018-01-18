#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common.h"

int is_quote(char ch) {
    return ch == '"' || ch == '\'';
}

int is_space(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

int get_end_of_string(char line[DATA_MAX_LEN], int len, int pos) {
    int end = pos;
    char ch = line[end];
    if (is_quote(ch)) {
        end = end + 1;
        while (end < len && line[end] != ch) {
            end = end + 1;
        }
    } else {
        while (end < len && !is_space(line[end])) {
            end = end + 1;
        }
    }
    return end;
}

char *cp_str(char line[DATA_MAX_LEN], int offset, int count) {
    char *ret = (char*) malloc(count + 1);
    int i = 0;
    while (i < count) {
        ret[i] = line[offset];
        i = i + 1;
        offset = offset + 1;
    }
    ret[i] = '\0';
    return ret;
}

int read_string(char line[DATA_MAX_LEN], int len, int pos, char **ret) {
    int end = get_end_of_string(line, len, pos);

    int size = end - pos;
    if (size > 0) {
        if (is_quote(line[pos])) {
            pos = pos + 1;
            size = size - 1;
        }
        *ret = cp_str(line, pos, size);
        pos = pos + size;
    }
    return pos;
}

int skip_spaces(char line[DATA_MAX_LEN], int len, int pos) {
    while (pos < len && is_space(line[pos])) {
        pos = pos + 1;
    }
    return pos;
}

void parse_line(char line[DATA_MAX_LEN], int len, int *argc, char *argv[DATA_MAX_LEN]) {
    int i = *argc;
    int ch = 0;
    while (ch < len) {
        ch = skip_spaces(line, len, ch);
        if (ch < len) {
            char *str;
            ch = read_string(line, len, ch, &str);
           if (str != NULL) {
                argv[i] = str;
                i = i + 1;
            }
        }
    }
    *argc = i;
}

void parse_file(char *name, int *paramc, char *params[DATA_MAX_LEN]) {
    FILE *f = fopen(name, "r");
    if (f != NULL) {
        char *line = NULL;
        size_t size = 0;
        int read = getline(&line, &size, f);
        while (feof(f) == 0) {
            parse_line(line, read, paramc, params);
            read = getline(&line, &size, f);
        }
        if (read > 0) {
            parse_line(line, read, paramc, params);
        }
        free(line); line = NULL;
    } else {
        fprintf(stderr, "unable to open file %s\n", name);
    }
}

int validate(int argc, char **argv) {
    int ret = 0;
    if (argc % 2 == 0) {
        int i = 0;
        // disable recursion: file shouldn't have -f key
        while (i < argc && strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] != 'f') {
            i = i + 2;
        }
        ret = i == argc;
    }
    return ret == 1;
}
