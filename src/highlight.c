#include <string.h>
#include "common.h"
#include "color.h"


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
