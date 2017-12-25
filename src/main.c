#include <stdio.h>
#include <string.h>

int LEN = 256;
int DATA_MAX_LEN = 256;
char esc = '\x1B';
char cyan = 36; // '\x24'
char decolor = 0;

int read(char rdbuf[], int buflen) {
	int rd = fread(rdbuf, 1, buflen-1, stdin);
	rdbuf[rd] = '\0';
	return rd;
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
	/* Итерация цикла:
	1. пропустить символы, с которых не начинается ни одна подстрока
	2. -> проверить, не является ли текущий символ началом искомой подстроки
	*/
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

void copy(char rdbuf[], int rdpos_prev, int rdpos_cur, char wrbuf[], int *wrpos_cur) {
	int len = rdpos_cur - rdpos_prev;
	char *dst = wrbuf + *wrpos_cur;
	char *src = rdbuf + rdpos_prev;
	memcpy(dst, src, len);
	*wrpos_cur = *wrpos_cur + len;
}

int emphase_substr(char str[], int len, char wrbuf[], int pos) {
	wrbuf[pos] = esc;
	wrbuf[pos + 1] = cyan;

	pos = pos + 2;
	char *dst = wrbuf + pos;
	memcpy(dst, str, len);
	pos = pos + len;

	wrbuf[pos] = esc;
	wrbuf[pos + 1] = decolor;
}

int emphase_line(char rdbuf[], int rdbuf_len, char wrbuf[], /*int wrbuflen,*/ char *data[], int data_len) {
   /* Итерация цикла:
   1. найти подстроку в буфере чтения
   2. скопировать префикс в буфер записи
   3. добавить цвет в буфер записи
   4. скопировать подстроку в буфер записи
   5. добавить исходный цвет в буфер записи
   */
	int rdpos_prev = 0, rdpos_cur = 0;
	int wrpos_cur = 0;
	int data_index = 0;
	find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	while (rdpos_cur < rdbuf_len) {
		char *d = data[data_index];
		int d_len = strlen(d);

		copy(rdbuf, rdpos_prev, rdpos_cur, wrbuf, &wrpos_cur);
		emphase_substr(d, d_len, wrbuf, wrpos_cur);

		rdpos_cur = rdpos_cur + d_len;
		rdpos_prev = rdpos_cur;

		find_substr(data, data_len, rdbuf, rdbuf_len, &rdpos_cur, &data_index);
	}
	copy(rdbuf, rdpos_prev, rdpos_cur, wrbuf, &wrpos_cur);
}

int write(const char wrbuf[], int buflen) {
	int wr = fwrite(wrbuf, 1, buflen, stdout);
	return wr;
}

/*
   args: { -s substring [-c color] | -w word [-c color] } | -f config_file
-s substring for emphase
-c terminal color
-w search for words only
*/
void main(int argc, char **argv) {
	char rdbuf[LEN];
	char wrbuf[LEN*5];
	char *data[DATA_MAX_LEN];

	int data_len = 0;
   while (feof(stdin) == 0) {
		int read = fread(rdbuf, 1, LEN, stdin);
		emphase_line(rdbuf, LEN, wrbuf, data, data_len);
		int written = fwrite(wrbuf, 1, read, stdout);
		//printf("\n===\nread:%i, written:%i\n", read, written);
		//printf("stdin:%i, stdout:%i\n", stdin, stdout);
	}
	printf("done\n");
}
