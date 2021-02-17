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

struct vector decmov(struct vector S, struct vector G,float length, int frame){
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
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    struct vector xy;
        xy.x =   S.x * cos(Theta*frame/length) + S.y * sin(Theta*frame/length);
        xy.y = - S.x * sin(Theta*frame/length) + S.y * cos(Theta*frame/length);
        xy.z = S.z;
    struct vector kur;
        kur.x = xy.x * cos(Phi*frame/length) - xy.z * sin(Phi*frame/length);
        kur.y = xy.y;
        kur.z = xy.x * sin(Phi*frame/length) + xy.z * cos(Phi*frame/length);
    return kur;
}


struct vector deckuru(struct vector S, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length;
    struct vector xy;
        xy.x =   S.x * cos(Theta*ratio*frame/length) + S.y * sin(Theta*ratio*frame/length);
        xy.y = - S.x * sin(Theta*ratio*frame/length) + S.y * cos(Theta*ratio*frame/length);
        xy.z = S.z;
        struct vector deck;
        deck.x = xy.x * cos(Phi*ratio*frame/length) - xy.z * sin(Phi*ratio*frame/length);
        deck.y = xy.y;
        deck.z = xy.x * sin(Phi*ratio*frame/length) + xy.z * cos(Phi*ratio*frame/length);
    return deck;
}

struct vector acckuru(struct vector S, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;
    double ratio = 1 - pow(2,-5*frame/length) + pow(2,-5)*frame/length;
    struct vector xy;
        xy.x =   S.x * cos(Theta*ratio*frame/length) + S.y * sin(Theta*ratio*frame/length);
        xy.y = - S.x * sin(Theta*ratio*frame/length) + S.y * cos(Theta*ratio*frame/length);
        xy.z = S.z;
        struct vector acck;
        acck.x = xy.x * cos(Phi*ratio*frame/length) - xy.z * sin(Phi*ratio*frame/length);
        acck.y = xy.y;
        acck.z = xy.x * sin(Phi*ratio*frame/length) + xy.z * cos(Phi*ratio*frame/length);
    return acck;
}
