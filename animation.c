#include <stdio.h>
#include <math.h>
#include "circle_gen.h"

struct vector S = {100,0,50};
struct vector G = {0,0,100};

struct vector cutin(struct vector S, struct vector G,int length, int frame){
    struct vector way;
        way.x = S.x - G.x,
        way.y = S.y - G.y,
        way.z = S.z - G.z;
        double prog = pow(2,-5 * frame % length);
            struct vector cin;
                cin.x = S.x + (prog * way.x),
                cin.y = S.y + (prog * way.y),
                cin.z = S.z + (prog * way.z);
            return cin;
}
