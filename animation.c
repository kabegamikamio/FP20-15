#include <stdio.h>
#include <math.h>
#include "img.h"

struct vector S = {100,0,50};
struct vector G = {0,0,100};

struct vector decmov(struct vector S, struct vector G,int length, int frame){
    struct vector way;
        way.x = S.x - G.x,
        way.y = S.y - G.y,
        way.z = S.z - G.z;
        double prog = pow(2,-5 * frame / length);
            struct vector dec;
                dec.x = S.x + (prog * way.x),
                dec.y = S.y + (prog * way.y),
                dec.z = S.z + (prog * way.z);
            return dec;
}
