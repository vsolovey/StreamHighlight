#include <string.h>
#include <stdlib.h>

// color = '<esc>[<num>m'
//const char esc = '\x1B'; '\e'


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

    colors[10] = dark_gray;
    colors[11] = light_red;
    colors[12] = light_green;
    colors[13] = light_yellow;
    colors[14] = light_blue;
    colors[15] = light_magneta;
    colors[16] = light_cyan;
    colors[17] = white;

    colors[20] = black_bg;
    colors[21] = red_bg;
    colors[22] = green_bg;
    colors[23] = yellow_bg;
    colors[24] = blue_bg;
    colors[25] = magneta_bg;
    colors[26] = cyan_bg;
    colors[27] = light_gray_bg;

    colors[30] = dark_gray_bg;
    colors[31] = light_red_bg;
    colors[32] = light_green_bg;
    colors[33] = light_yellow_bg;
    colors[34] = light_blue_bg;
    colors[35] = light_magneta_bg;
    colors[36] = light_cyan_bg;
    colors[37] = white_bg;
}

const char *get_color(char *num) {
    const char *color;
    int i = atoi(num);
    if (i >= 30 && i <= 37 || i >= 40 && i <= 47) {
        color = colors[i - 30];
    } else if (i >= 90 && i <= 97 || i >= 100 && i <= 107) {
        color = colors[i - 70];
    } else {
        color = cyan;
    }
    return color;
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
