#include <escape.h>
#include <unistd.h>

int main(void) {
    esc_color(esc_blue);
    puts("Some text");
    esc_color(esc_reset);

    const char str[] = "Hello, world!";
    for(int i = 0; i < (int) sizeof(str); i++) {
        esc_color((i % 6) + esc_red);
        putchar(str[i]);
    }
    esc_color(esc_reset);
    putchar('\n');

    printc("$c1Red $c2Green $c4Blue\n");

    printfc("$nSome %s\n", "color", esc_blue + esc_BRIGHT);

    esc_cursor(esc_right, sizeof(str) + 2);
    for(int i = sizeof(str); i >= 0; --i) {
        esc_color((i % 4) + esc_green + esc_BG);
        putchar(str[i]);

        // make the character appear immediatly
        fflush(stdout);

        usleep(70000);
        esc_cursor(esc_left, 2);
    }
    esc_color(esc_reset);
    putchar('\n');
    return 0;
}
