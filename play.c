#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"
// read in the game parameters from the command line
void read_argv(int argc, char* argv[], unsigned int* w, unsigned int* h,
               unsigned int* k, unsigned int* q, enum type *t) {
        *w = 0;
        *h = 0;
        *k = 0;
        *q = 0;
        unsigned int type_check = 0;;
        unsigned int i;
        for (i = 1; i < argc ; i++) {
                if (!(strcmp("-m", argv[i]))) {
                        *t = MATRIX;
                        type_check = 1;
                }
                else if (!(strcmp("-b", argv[i]))) {
                        *t = BITS;
                        type_check = 1;
                }
                if (!(strcmp("-w", argv[i]))) {
                        *w = atoi(argv[i+1]);
                }
                else if (!(strcmp("-h", argv[i]))) {
                        *h = atoi(argv[i+1]);
                }
                else if (!(strcmp("-k", argv[i]))) {
                        *k = atoi(argv[i+1]);
                }
                else if (!(strcmp("-q", argv[i]))) {
                        *q = atoi(argv[i+1]);
                }
        }
        if (!type_check) {
                fprintf(stdout, "Please specify matrix(-m) or bits(-b)"
                                "representation\n");
                exit(1);
        }
        if (!(*w && *h && *k && *q)) {
                fprintf(stdout,"Please input values for width(w), height(h), "
                                "stick length(k) and winning square size(q)\n");
                exit(1);
        }
}
// switch the current player (black to white or vice versa)
void turn_change(game* game) {
        switch (game->next) {
                case BLACK_NEXT:
                        game->next = WHITE_NEXT;
                        break;
                default:
                        game->next = BLACK_NEXT;
        }
}
// check if a number n is within a range a to b inclusive
int within(int a, int b, int n) {
        return (n >= a && n <= b);
}
 
// convert a header character to its column number
int convert_header(char c) {
        if (within(48, 57, (int)c)) 
                return atoi(&c);
        else if (within(65, 90, (int)c)) 
                return (int)(c-55);
        else if (within(97, 122, (int)c)) 
                return (int)(c-61);
        else 
                return -1;
}
// make a new game and run it 
void start_game(unsigned int w, unsigned int h, unsigned int k, unsigned int q,
                enum type type) {
        game* g = new_game(k, q, w, h, type);
        fprintf(stdout, "Welcome to the game\n");
        unsigned int round = 1;
        while(game_outcome(g) == IN_PROGRESS) {
                fprintf(stdout, "Round %u:\n", round);
                board_show(g->b);
                if (g->next == BLACK_NEXT) 
                        fprintf(stdout, "Black: ");
                else 
                        fprintf(stdout, "White: ");
                char c;
                unsigned int vert;
                fprintf(stdout, "\n\tMove: ");
                scanf("%c%*c", &c);
                if (c == '-') 
                        vert = 0;
                else if(c == '|') 
                        vert = 1;
                else if(c == '!') {
                        breakdown(g);
                }
                else {
                        fprintf(stdout, "Invalid character. Try again.\n");
                        fprintf(stdout, "Character: %c\n\n", c);
                        continue;
                }
                if ((c == '-') || (c == '|')) {
                        char col;
                        fprintf(stdout, "\tColumn: ");
                        scanf("%c%*c", &col);
                        int coln = convert_header(col);
                        if (!within(0, g->b->width, coln)) {
                                fprintf(stdout, "Column out of range." 
                                                " Try again.\n\n");
                                continue;
                        }
                        int drop = drop_stick(g, coln, vert);
                        if (!drop) {
                                fprintf(stdout, "Space full. Try again\n\n");
                                continue;
                        }
                }
                round++;
                turn_change(g);
                fprintf(stdout, "\n");
        }
        board_show(g->b);
        switch (game_outcome(g)) {
                case DRAW:
                        fprintf(stdout, "It's a draw!\n");
                        exit(1);
                case BLACK_WIN:
                        fprintf(stdout, "Black wins!\n");
                        exit(1);
                default:
                        fprintf(stdout, "White wins!\n");
                        exit(1);
        }
}
// make a new game and run it
int main(int argc, char* argv[]) {
        unsigned int w=0, h=0, k=0, q=0;
        enum type t;
        read_argv(argc, argv, &w, &h, &k, &q, &t);
        start_game(w, h, k, q, t);
        return 0;
}
