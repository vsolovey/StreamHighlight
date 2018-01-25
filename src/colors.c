#include <string.h>
#include <stdlib.h>

// color = '<esc>[<num>m'
//const char esc = '\x1B'; '\e'

#define _black 0
#define _red 1
#define _green 2
#define _yellow 3
#define _blue 4
#define _magneta 5
#define _cyan 6
#define _gray 7
#define _white 7

#define none (-1)
#define light 8
#define dark 9
#define bg 10

const char *black = "\e[30m";
const char *red = "\e[31m";
const char *green = "\e[32m";
const char *yellow = "\e[33m";
const char *blue = "\e[34m";
const char *magneta = "\e[35m";
const char *cyan = "\e[36m";
const char *light_gray = "\e[37m";

const char *dark_gray = "\e[90m";
const char *light_red = "\e[91m";
const char *light_green = "\e[92m";
const char *light_yellow = "\e[93m";
const char *light_blue = "\e[94m";
const char *light_magneta = "\e[95m";
const char *light_cyan = "\e[96m";
const char *white = "\e[97m";


const char *black_bg = "\e[40m";
const char *red_bg = "\e[41m";
const char *green_bg = "\e[42m";
const char *yellow_bg = "\e[43m";
const char *blue_bg = "\e[44m";
const char *magneta_bg = "\e[45m";
const char *cyan_bg = "\e[46m";
const char *light_gray_bg = "\e[47m";

const char *dark_gray_bg = "\e[100m";
const char *light_red_bg = "\e[101m";
const char *light_green_bg = "\e[102m";
const char *light_yellow_bg = "\e[103m";
const char *light_blue_bg = "\e[104m";
const char *light_magneta_bg = "\e[105m";
const char *light_cyan_bg = "\e[106m";
const char *white_bg = "\e[107m";

const char *decolor = "\e[0m";

static const char *colors[40];

void colors_init() {
    colors[0] = black;
    colors[1] = red;
    colors[2] = green;
    colors[3] = yellow;
    colors[4] = blue;
    colors[5] = magneta;
    colors[6] = cyan;
    colors[7] = light_gray;

    colors[10] = black_bg;
    colors[11] = red_bg;
    colors[12] = green_bg;
    colors[13] = yellow_bg;
    colors[14] = blue_bg;
    colors[15] = magneta_bg;
    colors[16] = cyan_bg;
    colors[17] = light_gray_bg;

    colors[20] = dark_gray;
    colors[21] = light_red;
    colors[22] = light_green;
    colors[23] = light_yellow;
    colors[24] = light_blue;
    colors[25] = light_magneta;
    colors[26] = light_cyan;
    colors[27] = white;

    colors[30] = dark_gray_bg;
    colors[31] = light_red_bg;
    colors[32] = light_green_bg;
    colors[33] = light_yellow_bg;
    colors[34] = light_blue_bg;
    colors[35] = light_magneta_bg;
    colors[36] = light_cyan_bg;
    colors[37] = white_bg;
}

int continues(char *str, int *pos, char *pattern, int result) {
    int ret = none;
    char *tmp = str + *pos;
    int len = strlen(pattern);
    if (memcmp(tmp, pattern, len) == 0) {
        ret = result;
        *pos = *pos + len;
    }
    return ret;
}

int parse_prefix(char *str, int *pos) {
    int pref = continues(str, pos, "light", light);
    if (pref == none) { pref = continues(str, pos, "dark", dark); }
    return pref;
}

int parse_color_name(char *str, int *pos) {
    int color = continues(str, pos, "red", _red);
    if (color == none) {
        color = continues(str, pos, "green", _green);
        if (color == none) { color = continues(str, pos, "cyan", _cyan); }
        if (color == none) { color = continues(str, pos, "yellow", _yellow); }
        if (color == none) {
            color = continues(str, pos, "blue", _blue);
            if (color == none) { color = continues(str, pos, "magneta", _magneta); }
            if (color == none) {
                color = continues(str, pos, "white", _white);
                if (color == none) { color = continues(str, pos, "gray", _gray); }
                if (color == none) { color = continues(str, pos, "black", _black); }
            }
        }
    }
    return color;
}

int resolve_alias(char *alias) {
    int num = 0;
    int pos = 0;
    int pref, color, post;
    pref = parse_prefix(alias, &pos);
    if (pref != none) {
        pos = pos + 1;
    }
    color = parse_color_name(alias, &pos);
    post = continues(alias, &pos, "_bg", bg);

    if (pref == dark && color == _gray) {
        color = _black;
    } else if (pref == light && color == _gray) {
        pref = none;
    } else if (color == _white) {
        pref = light;
    }
    if (color != none) {
        num = 30 + color;
        if (pref != none) {
            num = num + 60;
        }
        if (post != none) {
            num = num + 10;
        }
    } else {
        num = 30 + _cyan;
    }
    return num;
}

const char *prepare_color(int i) {
    const char *color;
    if (i >= 30 && i <= 37 || i >= 40 && i <= 47) {
        color = colors[i - 30];
    } else if (i >= 90 && i <= 97 || i >= 100 && i <= 107) {
        color = colors[i - 70];
    } else {
        color = cyan;
    }
    return color;
}

const char *get_color(char *num) {
    int i = atoi(num);
    if (i == 0 && strlen(num) > 0) {
        i = resolve_alias(num);
    }
    return prepare_color(i);
}

/*char *get_compo_color(char *fg, char *bg) {
    int f = 0, b = 0;
    int key = 0;
    if (fg != NULL) {
        f = atoi(fg);
    }
    if (bg != NULL) {
        b = atoi(bg);
    }
    char *color;
    if (b == 0) {
        color = get_default_color(f);
    } else if (f == 0) {
        color = get_default_color(b);
    } else {

    }
    return color;
}*/
