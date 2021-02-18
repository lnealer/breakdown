#include <stdio.h>
#include <stdlib.h>
#include "logic.h"
game* new_game(unsigned int stick, unsigned int square, unsigned int width,
                unsigned int height, enum type type) {
        game* new = (game*)malloc(sizeof(game));
        if (new == NULL) {
                fprintf(stderr, "new_game: allocation failed\n");
                exit(1);
        }
        new->stick= stick;
        new->square=square;
        board* b = board_new(width, height, type);
        new->b = b;
        new->next = BLACK_NEXT;
        return new;
}
void game_free(game* g) {
        board_free(g->b);
        free(g);
}
unsigned int is_empty(board* b, pos p) {
        switch (board_get(b, p)) {
                        case EMPTY:
                                return 1;
                        default:
                                return 0;
        }
}        
// return if a stick can be dropped, but don't drop it
// room is an out param of the amount of space to drop
int drop_stick_true(game* g, unsigned int col, int vertical, 
                unsigned int* room) {
        cell cell;
        *room = 0;
        switch (g->next) {
                case BLACK_NEXT:
                        cell = BLACK;
                        break;
                case WHITE_NEXT: 
                        cell = WHITE;
        }
        if (col < 0 || col >= g->b->width) {
                return 0;
        }
        if (vertical) {
                pos p = make_pos(0, col);
                while(is_empty(g->b, p)) {
                        (*room)++;
                        p.r++;
                        if (p.r == g->b->height) {
                                break;
                        }
                }
                if (*room < g->stick) {
                        return 0;
                }
                return 1;
        }
        else {
                if (col+g->stick > g->b->width) 
                        return 0;
                unsigned int c = 0, r = 0;
                for (r = 0 ; r < g->b->height ; r++) {
                        for (c = 0; c < g->stick ; c++) {
                                pos cur = make_pos(r, col+c);
                                if (!is_empty(g->b, cur)) {
                                        c++;
                                        *room = r;
                                        break;
                                }
                        }
                        c--;
                        if (!is_empty(g->b, make_pos(r,c+col))) {
                                break;        
                        }
                }
                *room = r; 
                if (!(*room)) {
                       return 0;
                }
                return 1;
        }
}
int drop_stick(game* g, unsigned int col, int vertical) {
        cell cell;
        switch (g->next) {
                case BLACK_NEXT:
                        cell = BLACK;
                        break;
                case WHITE_NEXT: 
                        cell = WHITE;
        }
        unsigned int room = 0;
        if (drop_stick_true(g, col, vertical, &room)) {
                if (vertical) {
                        for (unsigned int i = 1; i <= g->stick  ; i++) {
                                pos cur = make_pos(room-i, col);
                                board_set(g->b, cur, cell);
                        }
                        return 1;
                }
                else {
                        for (unsigned int k = col ; k < col+g->stick ; k++) {
                                pos cur = make_pos(room-1, k);  
                                board_set(g->b, cur, cell);
                        }                
                        return 1;
                }
        }
        else {
                return 0;
        }
}
void breakdown(game* g) {
        for (unsigned int c = 0 ; c < g->b->width ; c++) {
                unsigned int empty_below = 0;
                pos p = make_pos(0, 0);
                pos first_empty = make_pos(g->b->height, g->b->width);
                for (int r = g->b->height-1 ; r >= 0 ; r--) {
                        p.r = r;
                        p.c = c;
                        if (is_empty(g->b, p)) {
                                if (!empty_below) {
                                        first_empty.r=r;
                                        first_empty.c=c;
                                        empty_below = 1;
                                }
                        }
                        else        {
                                if (empty_below) {
                                        cell cur = board_get(g->b, p);
                                        board_set(g->b, first_empty, cur);
                                        board_set(g->b, p, EMPTY);
                                        if (first_empty.r > 0) {
                                                first_empty.r--;
                                        }
                                }
                        }
                }
        }
}
// check if there are any squares on the board
outcome find_square(game* g, pos p) {
        if (p.c + g->square > g->b->width) {
                return IN_PROGRESS;
        }
        if (p.r + g->square > g->b->height) {
                return IN_PROGRESS;
        }
        unsigned int b_size = 0, w_size = 0;
        for (unsigned int r = 0; r < g->square ; r++){
                for (unsigned int c = 0 ; c < g->square ; c++) {
                        pos cur = make_pos(p.r+r, p.c+c);
                        if (board_get(g->b, cur) == BLACK) {
                               b_size++;
                        }
                         if (board_get(g->b, cur) == WHITE) {
                                w_size++;
                        }
                }
        }
        unsigned int area = g->square * g->square;
        if (b_size == area) {
                return BLACK_WIN;
        }
        else if (w_size == area) {
                return WHITE_WIN;
        }
        else {
                return IN_PROGRESS;
        }
}
// check if a given position is a 'ledge', ie overhangs empty space        
unsigned int is_ledge(game* g, pos p) {
        if (p.r >= g->b->height - 1) {
                return 0;
        }
        else {
                pos below = make_pos(p.r+1, p.c);
                return (((!is_empty(g->b, p))) && is_empty(g->b, below));
        }
}
outcome game_outcome(game* g) {
        unsigned int c=0, ledges = 0, any_room = 0, b_wins = 0, w_wins = 0;
        for (c = 0; c < g->b->width ; c++) {
                for (unsigned int r = 0; r < g->b->height ; r++) {
                        pos cur = make_pos(r, c);
                        ledges += is_ledge(g, cur);
                        outcome any_sqrs = find_square(g,cur);
                        if (any_sqrs == WHITE_WIN) {
                                w_wins++;
                        }
                        else if(any_sqrs == BLACK_WIN) {
                                b_wins++;
                        }
                }
                unsigned int room = 0;
                any_room += drop_stick_true(g, c, 1, &room); 
                any_room += drop_stick_true(g, c, 0, &room);
        }
        if (b_wins && w_wins) {
                return DRAW;
        }
        else if (b_wins) {
                return BLACK_WIN;
        }
        else if (w_wins) {
                return WHITE_WIN;
        }
        else if (any_room) {
                return IN_PROGRESS;
        }
        else if (ledges) {
                return IN_PROGRESS;
        }
        else {
                return DRAW;
        }
}
