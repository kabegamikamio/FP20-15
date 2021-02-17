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
        double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
            struct vector decm;
                decm.x = S.x + (ratio * way.x),
                decm.y = S.y + (ratio * way.y),
                decm.z = S.z + (ratio * way.z);
            return decm;
}

struct vector kurukuru(struct vector S, double theta ,double phi, int length, int frame){
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


struct vector deckuru(struct vector S, double theta ,double phi, int length, int frame){
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
/*
int main(){
    int i;
    struct vector S = {0, 0, 100};
    struct vector G = {0, 0, 0};
        for(i = 0;i <= 10; i++){
        img_clear();
        struct vector Decm = decmov(S, G, 10, i);
        P1.x = Decm.x, P1.y = Decm.y, P1.z = Decm.z;
        hit_test();
        img_write();
        }
    return 0;
}
*/