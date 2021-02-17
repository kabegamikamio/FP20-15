#include <stdio.h>
#include <math.h>
#include "img.h"

struct vector S = {100,0,50};
struct vector G = {0,0,100};

struct vector accmov(struct vector S, struct vector G,float length, int frame){
    struct vector way;
        way.x = G.x - S.x,
        way.y = G.y - S.y,
        way.z = G.z - S.z;
        double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
            struct vector accm;
                accm.x = S.x + (ratio * way.x),
                accm.y = S.y + (ratio * way.y),
                accm.z = S.z + (ratio * way.z);
            return accm;
}

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

struct vector kurukuru(struct vector S, struct vector O, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    struct vector S1;
        S1.x = S.x -O.x,
        S1.y = S.y -O.y,
        S1.z = S.z -O.z;
    struct vector xy;
        xy.x =   S1.x * cos(Theta*frame/length) + S1.y * sin(Theta*frame/length);
        xy.y = - S1.x * sin(Theta*frame/length) + S1.y * cos(Theta*frame/length);
        xy.z = S1.z;
    struct vector xz;
        xz.x = xy.x * cos(Phi*frame/length) - xy.z * sin(Phi*frame/length);
        xz.y = xy.y;
        xz.z = xy.x * sin(Phi*frame/length) + xy.z * cos(Phi*frame/length);
    struct vector Kur;
        Kur.x = xz.x + O.x,
        Kur.y = xz.y + O.y,
        Kur.z = xz.z + O.z;
    return Kur;
}


struct vector deckuru(struct vector S, struct vector O, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
    struct vector S1;
        S1.x = S.x -O.x,
        S1.y = S.y -O.y,
        S1.z = S.z -O.z;
    struct vector xy;
        xy.x =   S1.x * cos(Theta*ratio*frame/length) + S1.y * sin(Theta*ratio*frame/length);
        xy.y = - S1.x * sin(Theta*ratio*frame/length) + S1.y * cos(Theta*ratio*frame/length);
        xy.z = S1.z;
        struct vector xz;
        xz.x = xy.x * cos(Phi*ratio*frame/length) - xy.z * sin(Phi*ratio*frame/length);
        xz.y = xy.y;
        xz.z = xy.x * sin(Phi*ratio*frame/length) + xy.z * cos(Phi*ratio*frame/length);
    struct deck;
        deck.x = xz.x +O.x,
        deck.y = xz.y +O.y,
        deck.z = xz.z +O.z;
    return deck;
}

struct vector acckuru(struct vector S, struct vector O, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    double ratio = 1 - pow(2,-5*frame/length) + pow(2,-5)*frame/length;
    struct vector S1;
        S1.x = S.x -O.x,
        S1.y = S.y -O.y,
        S1.z = S.z -O.z;
    struct vector xy;
        xy.x =   S1.x * cos(Theta*ratio*frame/length) + S1.y * sin(Theta*ratio*frame/length);
        xy.y = - S1.x * sin(Theta*ratio*frame/length) + S1.y * cos(Theta*ratio*frame/length);
        xy.z = S1.z;
    struct vector xz;
        xz.x = xy.x * cos(Phi*ratio*frame/length) - xy.z * sin(Phi*ratio*frame/length);
        xz.y = xy.y;
        xz.z = xy.x * sin(Phi*ratio*frame/length) + xy.z * cos(Phi*ratio*frame/length);
    struct vector acck;
        acck.x = xz.x +O.x,
        acck.y = xz.y +O.y,
        acck.z = xz.z +O.z;
    return acck;
}
