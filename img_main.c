/**********************************
 * Ray-tracing animation for FP20 *
 *                                *
 *           Created by           *
 *      "Team Late-Racing"        *
 *           Takumi.N             *
 *    Yuuki.S        Sota.K       *
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img.h"

static unsigned char buf[HEIGHT][WIDTH][3];
static int filecnt = 0;
static char fname[100];
static int a = 499;     //視点から画素までの距離
struct vector light = {-1000, 2000, 0};     //光源の座標
struct vector P = {1000, 0, 100};   //球の中心座標 あとで消す

//配列bufを白ピクセルですべて埋める関数
void img_clear(void){
    int i, j;
    for(j = 0; j < HEIGHT; j++){
        for(i = 0; i < WIDTH; i++){
            buf[j][i][0] = buf[j][i][1] = buf[j][i][2] = 255;
        }
    }
}

//配列bufを画像ファイルに写す関数
void img_write(void){
    sprintf(fname, ".\\image\\img%04d.ppm", filecnt++);
    FILE *f = fopen(fname, "wb");
    if(f == NULL){
        fprintf(stderr, "can't open %s\n", fname);
        exit(1);
    }
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(buf, sizeof(buf), 1, f);
    fclose(f);
}

//配列bufに対してピクセルごとに色情報を記録させる関数
void img_putpixel(struct color c, int x, int y){
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    buf[HEIGHT-y-1][x][0] = c.r;
    buf[HEIGHT-y-1][x][1] = c.g;
    buf[HEIGHT-y-1][x][2] = c.b;
}

//内積関数
double dot(struct vector a, struct vector b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//外積関数
struct vector product(struct vector a, struct vector b){
    struct vector ret;
        ret.x = a.y * b.z - a.z * b.y,
        ret.y = a.z * b.x - a.x * b.z,
        ret.z = a.x * b.y - a.y * b.x;
    return ret;
}

//正規化関数
struct vector normalize(struct vector a){
    double norm = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
    a.x = a.x / norm,
    a.y = a.y / norm,
    a.z = a.z / norm;
    return a;
}

//位置ベクトルaを回転させる関数
struct vector rotation(struct vector a, double theta, double phi){
    struct vector xz;
        xz.x =   a.x * cos(theta) + a.y * sin(theta);
        xz.y = - a.x * sin(theta) + a.y * cos(theta);
        xz.z = a.z;
        struct vector rot;
        rot.x = xz.x * cos(phi) - xz.z * sin(phi);
        rot.y = xz.y;
        rot.z = xz.x * sin(phi) + xz.z * cos(phi);
    return rot;
}

//正反射ベクトルを求める関数
struct vector reflect(struct vector a, struct vector n){
    struct vector N = normalize(n);
    double mag;
        mag = dot(a,N);
    struct vector reflect;
        reflect.x = a.x + 2 * mag * N.x;
        reflect.y = a.y + 2 * mag * N.y;
        reflect.z = a.z + 2 * mag * N.z;
    return reflect;
}

//あるベクトルと平面の交点を求める関数
struct vector cross_point(struct vector p, struct vector v, struct vector q, struct vector n){
    //p,vはそれぞれ直線の一点、方向ベクトル
    //q,nはそれぞれ平面の一点、法線ベクトル
    double t = fabs(n.x*(p.x-q.x)+n.y*(p.y-q.y)+n.z*(p.z-q.z))/dot(v,n);
    struct vector cross_point = {
            p.x + t*v.x,
            p.y + t*v.y,
            p.z + t*v.z
    };
    return cross_point;
}

//野生の一般main関数
int main(){
    int i;
    for(i = 0; i < 10; i++){
        img_clear();
        hit_test();
        img_write();
        P.z -= 10 * i;
    }
    return 0;
}