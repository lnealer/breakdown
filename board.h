#ifndef BOARD_H
#define BOARD_H
#include "pos.h"
enum cell {
        EMPTY,
        BLACK,
        WHITE
};
typedef enum cell cell;
union board_rep {
        enum cell** matrix;
        unsigned int* bits;
};
typedef union board_rep board_rep;
enum type {
        MATRIX, BITS
};
struct board {
        unsigned int width, height;
        enum type type;
        board_rep u;
};
typedef struct board board;
// make a new board struct
board* board_new(unsigned int width, unsigned int height, enum type type);
// free a board struct
void board_free(board* b);
// print a board to the console
void board_show(board* b);
// retrieve a cell at pos p on the board
cell board_get(board* b, pos p);
// change a cell on the board at pos p 
void board_set(board* b, pos p, cell c);
#endif /* BOARD_H */
