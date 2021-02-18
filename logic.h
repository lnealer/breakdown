#ifndef LOGIC_H
#define LOGIC_H
#include "board.h"
enum turn {
        BLACK_NEXT,
        WHITE_NEXT
};
typedef enum turn turn;
enum outcome {
        BLACK_WIN,
        WHITE_WIN,
        DRAW,
        IN_PROGRESS
};
typedef enum outcome outcome; 
struct game {
        unsigned int stick, square;
        board* b;
        turn next;
}; 
typedef struct game game;
// make a new game value
game* new_game(unsigned int stick, unsigned int square, unsigned int width,
                unsigned int height, enum type type);
// free a game
void game_free(game* g);
// drop a stick of the player whose turn it is 
int drop_stick(game* g, unsigned int col, int vertical);
// sticks disintegrate into constituent squares and drop
void breakdown(game* g);
// report the outcome of a completed game or if the game is in progress
outcome game_outcome(game*g);
#endif /* LOGIC_H */
