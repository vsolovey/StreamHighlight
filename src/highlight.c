#include <string.h>
#include "common.h"
#include "colors.h"


int wordpart(char ch) {
	return ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' && ch != '\0';
}

int matched_type(elem e, char ch, int is_space_before) {
	return e.type == SUBSTRING && e.str[0] == ch || e.type == WORD && is_space_before && e.str[0] == ch;
}

/* candidate - это строка, начинающаяся с символа ch
ch - first character of a string
data - array of strings
data_len - length of data
data_pos - start position for looking for candidates
returns index of candidate. If the index equals data_len, then data have no candidates.
*/
int get_candidate(char ch, elem data[], int data_len, int data_pos, int is_space_before) {
	while (data_pos < data_len && !matched_type(data[data_pos], ch, is_space_before)) {
		data_pos = data_pos + 1;
	}
	return data_pos;
}

int matched_str(elem *e, char buf[], int pos, int buf_len) {
	char *string = buf + pos;
	int len = buf_len - pos;
	int i = 0;
	while (i < len && e->str[i] == string[i]) {
		i = i + 1;
	}
	return e->str[i] == '\0' && (e->type == SUBSTRING || e->type == WORD && !wordpart(string[i]));
}

void find_substr(elem data[DATA_MAX_LEN], int data_len, char rdbuf[], int rdbuf_len, int *rdpos_cur, int *data_index) {
	int i = -1;
	int start_from = 0;
	int pos = *rdpos_cur;
	int is_space_before = !wordpart(' ');
	while (pos < rdbuf_len && i == -1) {
		int candidate = get_candidate(rdbuf[pos], data, data_len, start_from, is_space_before);
		if (candidate == data_len) { // no candidates found
			is_space_before = !wordpart(rdbuf[pos]);
			pos = pos + 1;
		} else {
			while (candidate < data_len && !matched_str(&data[candidate], rdbuf, pos, rdbuf_len)) {
				start_from = candidate + 1;
				candidate = get_candidate(rdbuf[pos], data, data_len, start_from, is_space_before);
			}
			if (candidate < data_len) {
				i = candidate;
				*data_index = candidate;
			} else {
				is_space_before = !wordpart(rdbuf[pos]);
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

void prepare_and_copy(char *from, int from_prev, int from_cur, char *to, int *to_base) {
	char *tmp = from + from_prev;
	int tmp_len = from_cur - from_prev;
	copy(tmp, tmp_len, to, to_base);
}

int emphase_substr(char str[], char *color, int str_len, char wrbuf[], int *pos) {
	copy(color, strlen(color), wrbuf, pos);
	copy(str, str_len, wrbuf, pos);
	copy(decolor, strlen(decolor), wrbuf, pos);
}

int emphase_line(char rdbuf[], int rdbuf_len, char wrbuf[], elem data[DATA_MAX_LEN], int data_len) {
	int rdpos_prev = 0, rdpos_cur = 0;
	int wrpos_cur = 0;
	int data_index = 0;
	find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	while (rdpos_cur < rdbuf_len) {
		char *d = data[data_index].str;
		int d_len = strlen(d);

		prepare_and_copy(rdbuf, rdpos_prev, rdpos_cur, wrbuf, &wrpos_cur);
		emphase_substr(d, data[data_index].color, d_len, wrbuf, &wrpos_cur);

		rdpos_cur = rdpos_cur + d_len;
		rdpos_prev = rdpos_cur;

		find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	}
	prepare_and_copy(rdbuf, rdpos_prev, rdpos_cur, wrbuf, &wrpos_cur);
	return wrpos_cur;
}
