#include <stdio.h>
#include <math.h>
#include "img.h"

struct vector S = {100,0,50};
struct vector G = {0,0,100};

struct vector decmov(struct vector S, struct vector G, float length, int frame){
    struct vector way;
        way.x = S.x - G.x,
        way.y = S.y - G.y,
        way.z = S.z - G.z;
        double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
            struct vector decm;
                decm.x = G.x + (ratio * way.x),
                decm.y = G.y + (ratio * way.y),
                decm.z = G.z + (ratio * way.z);
            return decm;
}

struct vector kurukuru(struct vector S, double theta ,double phi, float length, int frame){
    struct vector xy;
        xy.x =   S.x * cos(theta*frame/length) + S.y * sin(theta*frame/length);
        xy.y = - S.x * sin(theta*frame/length) + S.y * cos(theta*frame/length);
        xy.z = S.z;
        struct vector kur;
        kur.x = xy.x * cos(phi*frame/length) - xy.z * sin(phi*frame/length);
        kur.y = xy.y;
        kur.z = xy.x * sin(phi*frame/length) + xy.z * cos(phi*frame/length);
    return kur;
}


struct vector deckuru(struct vector S, double theta ,double phi, float length, int frame){
    double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
    struct vector xy;
        xy.x =   S.x * cos(theta*ratio*frame/length) + S.y * sin(theta*ratio*frame/length);
        xy.y = - S.x * sin(theta*ratio*frame/length) + S.y * cos(theta*ratio*frame/length);
        xy.z = S.z;
        struct vector deck;
        deck.x = xy.x * cos(phi*ratio*frame/length) - xy.z * sin(phi*ratio*frame/length);
        deck.y = xy.y;
        deck.z = xy.x * sin(phi*ratio*frame/length) + xy.z * cos(phi*ratio*frame/length);
    return deck;
}
