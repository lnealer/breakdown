#include <stdlib.h>
#include <stdio.h>
#include "pos.h"
pos make_pos(unsigned int r, unsigned int c) {
        pos new; 
        new.r = r;
        new.c = c;
        return new;
}
