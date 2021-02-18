#include <stdio.h>
#include <stdlib.h>
#include "board.h"
// return the ceiling of the division x/y
unsigned int ceiling(unsigned int x, unsigned int y) {
        unsigned int res = x/y + (x%y != 0);
        return res;
}
board* board_new(unsigned int width, unsigned int height, enum type type) {
        board* new = (board*)malloc(sizeof(board));
        if (new == NULL) {
                fprintf(stderr, "board_new: allocation failed\n");
                exit(1);
        }
        new->width = width;
        new->height = height;
        new->type = type;
        if (type == MATRIX) {
                cell** matrix =(cell**)malloc(sizeof(cell*)*height);
                for (unsigned int r = 0; r < height ; r++) {
                        matrix[r] = (cell*)malloc(sizeof(cell)*width);
                        if (matrix[r] == NULL) {
                                fprintf(stderr, "board_new: allocation failed\n");
                                exit(1);
                        }
                        for (unsigned int c = 0; c < width ; c++) {
                                matrix[r][c] = EMPTY;
                        }
                }
                if (matrix == NULL) {
                        fprintf(stderr, "board_new: allocation failed\n");
                        exit(1);
                }
                new->u.matrix = matrix;
        }
        else {
                unsigned int* bits;
                unsigned int area = width*height; 
                unsigned int bit_size = ceiling(area, 16);
                bits = (unsigned int*)malloc(sizeof(unsigned int)*bit_size); 
                if (!bits) {
                        fprintf(stderr, "board_new: allocaiton failed\n");
                        exit(1);
                }
                for (unsigned int i = 0 ; i < bit_size ; i++) {
                        bits[i] = 0;
                }
                new->u.bits = bits;
        }
        return new;
}
void board_free(board* b) {
        if (b->type == MATRIX) {
                for (unsigned int r = 0; r < b->height ; r++) {
                        free(b->u.matrix[r]);
                }
                free(b->u.matrix);
        }
        else {
                free(b->u.bits);
        }        
        free(b);
}
// convert an int to a header: 
//first numbers 0-9, then capital alphabet, lowercase alphabet, then '?'
void print_header(unsigned int h) {
        if (h < 10) {
                printf("%u", h);
        }
        else if (h < 36) {
                printf("%c", h+55);
        }
        else if (h < 62) {
                printf("%c", h+61);
        }
        else {
                printf("?");
        }
}
// return the index of the bits array where the pos is located on the board
unsigned int index_find(board* b, pos p) {
        unsigned int spot = (b->width * p.r) + p.c + 1;
        unsigned int res = spot/16;
        return res;
}
cell board_get(board* b, pos p) {
        if (p.r >= b->height) {
                fprintf(stderr, "board_get: requested row is off board\n");
                printf("row: %u\n", p.r);
                exit(1);
        }
        if (p.c >= b->width) {
                fprintf(stderr, "board_get: requested column is off board\n");
                printf("column: %u\n", p.c);
                exit(1);
        }
        if (b->type == MATRIX) {
                cell** matrix = b->u.matrix;
                cell c = matrix[p.r][p.c];
                return c;
        }
        else {
                unsigned int* bits = b->u.bits;
                unsigned int i = index_find(b, p);
                unsigned int shift_value = 2*((b->width*p.r+p.c+1)%16-1); 
                unsigned int mask = 3 << shift_value;
                unsigned int res = (bits[i] & mask) >> shift_value;
                if (!res) 
                        return EMPTY;
                else if (res % 2) 
                        return BLACK;
                
                else 
                        return WHITE;
        }
}
void board_show(board* b) {
        printf("  ");
        for (unsigned int h = 0 ; h < b->width ; h++) {
                print_header(h);
        }
        printf("\n\n");
        for (unsigned int r = 0; r < b->height ; r++) {
                print_header(r);
                printf(" ");
                for (unsigned int c = 0 ; c < b->width ; c++) {
                        pos p = make_pos(r, c);
                        switch(board_get(b, p)) {
                                case EMPTY:
                                        printf(".");
                                        break;
                                case WHITE:
                                        printf("o");
                                        break;
                                case BLACK:
                                        printf("*");
                                        break;
                                default:
                                        fprintf(stderr, "not a valid type\n");
                                        exit(1);
                        }
                        if (c == b->width-1) {
                                printf("\n");
                        }
                }
        }
        printf("\n");
}
                
void board_set(board* b, pos p, cell c) {
        if (p.r >= b->height) {
                fprintf(stderr, "board_set: requested row is off board\n");
                printf("row: %u\n", p.r);
                exit(1);
        }
        if (p.c >= b->width+1) {
                fprintf(stderr, "board_set: requested column is off board\n");
                printf("column: %u\n", p.c);
                exit(1);
        }
        if (b->type == MATRIX)
                b->u.matrix[p.r][p.c] = c;
        else {
                unsigned int mask1 = 3, mask2;
                if (c == EMPTY) 
                        mask2 = 0;
                else if (c == BLACK) 
                        mask2 = 1;
                else
                        mask2 = 2;
                unsigned int i = index_find(b, p);
                unsigned int shift_value = 2 * ((b->width*p.r+p.c+1)%16-1); 
                mask1 = ~(mask1 << shift_value);
                mask2 = mask2 << shift_value;
                b->u.bits[i] = (b->u.bits[i] & mask1) | mask2;
        }
}
