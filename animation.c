#include <stdio.h>
#include <math.h>
#include "img.h"

struct vector S = {700,50,50};
struct vector G = {700,50,100};
struct vector O = {700,0,100};

struct vector accmov(struct vector S, struct vector G,float length, int frame){
    struct vector way;
        way.x = G.x - S.x,
        way.y = G.y - S.y,
        way.z = G.z - S.z;
                         //始点から終点への方向ベクトル。S(tart) + way = G(oal)
        double ratio = pow(2,-5*frame/length) - pow(2,-5)*frame/length; 
/*加速度を導入するのは面倒なので、(1,0)(0,1)を通るそれっぽい形の曲線を0~1の範囲で分割し、
    その値を代入するようにした。なんかエフェクトのコンプっぽいねと思ったのでratio。*/
            struct vector accm;
                accm.x = S.x + (ratio * way.x),
                accm.y = S.y + (ratio * way.y),
                accm.z = S.z + (ratio * way.z);
            return accm;
}

struct vector decmov(struct vector S, struct vector G, float length, int frame){
    //accと同じことを逆向きにしただけ。
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
    double Phi = phi * M_PI/180;    //度数→radへの変換。
    struct vector S1;               //平行移動し、回転中心を原点に持ってくる。
        S1.x = S.x -O.x,
        S1.y = S.y -O.y,
        S1.z = S.z -O.z;
    struct vector xy;
        xy.x =   S1.x * cos(Theta*frame/length) + S1.y * sin(Theta*frame/length);
        xy.y = - S1.x * sin(Theta*frame/length) + S1.y * cos(Theta*frame/length);
        xy.z = S1.z;                //z軸を軸とし、xy平面に回転。
    struct vector xz;
        xz.x = xy.x * cos(Phi*frame/length) - xy.z * sin(Phi*frame/length);
        xz.y = xy.y;                //y軸を軸とし、xz平面に回転。
        xz.z = xy.x * sin(Phi*frame/length) + xy.z * cos(Phi*frame/length);
    struct vector Kur;
        Kur.x = xz.x + O.x,
        Kur.y = xz.y + O.y,
        Kur.z = xz.z + O.z;         //平行移動した分を修正。
    return Kur;
}


struct vector deckuru(struct vector S, struct vector O, double theta ,double phi, float length, int frame){
    double Theta = theta * M_PI/180;
    double Phi = phi * M_PI/180;    //kurukuruとacc/decの組み合わせ。回転角度にratioをかけることで、擬似的に角速度の変化を再現。
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
    double Phi = phi * M_PI/180;       //deckuruと同じ。
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
