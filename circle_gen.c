#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img.h"

static int a = 50;
static unsigned char buf[HEIGHT][WIDTH][3];
static int filecnt = 0;
static char fname[100];
static struct vector light = {20, 0, 50};

//White out the image
void img_clear(void){
    int i, j;
    for(j = 0; j < HEIGHT; j++){
        for(i = 0; i < WIDTH; i++){
            buf[j][i][0] = buf[j][i][1] = buf[j][i][2] = 255;
        }
    }
}

//Generate image file
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

void img_putpixel(struct color c, int x, int y){
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    buf[HEIGHT-y-1][x][0] = c.r;
    buf[HEIGHT-y-1][x][1] = c.g;
    buf[HEIGHT-y-1][x][2] = c.b;
}

void img_fillcircle(struct color c, double x, double y, double r){
    int imin = (int)(x-r-1), imax = (int)(x+r+1);
    int jmin = (int)(y-r-1), jmax = (int)(y+r+1);
    int i, j;
    for(j = jmin; j <= jmax; j++){
        for(i = imin; i <= imax; i++){
            if((x-i)*(x-i) + (y-j)*(y-j) <= r*r){
                img_putpixel(c, i, j);
            }
        }
    }
}

double dot(struct vector a, struct vector b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vector product(struct vector a, struct vector b){
    struct vector ret;
        ret.x = a.y * b.z - a.z * b.y,
        ret.y = a.z * b.x - a.x * b.z,
        ret.z = a.x * b.y - a.y * b.x;
    return ret;
}

struct vector normalize(struct vector a){
    double norm = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
    a.x = a.x / norm,
    a.y = a.y / norm,
    a.z = a.z / norm;
    return a;
}

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

struct color distance_ray(struct color RGB, struct vector L, struct vector C){ //光線と表面の交点Cと光源Lの距離から明るさを
    struct color ret = RGB;
    double k = 2;
    double dist = sqrt(pow(L.x - C.x, 2) + pow(L.y - C.y, 2) + pow(L.z - C.z, 2)) / 700;
    if(dist != 0){
        ret.r = (unsigned char)(k * (double)RGB.r / pow(dist, 2)), 
        ret.g = (unsigned char)(k * (double)RGB.g / pow(dist, 2)), 
        ret.b = (unsigned char)(k * (double)RGB.b / pow(dist, 2));
    }
    return ret;
}


/*
    *****ベクトルは単位ベクトルとして与えること！*****
    N : 反射表面の法線ベクトル
    L : 光源の方向ベクトル
    V : 視点の方向ベクトル
    Cs : 表面の色(RGB)
*/
struct color phong(struct vector N, struct vector L, struct vector V, struct color Cs){
    double  kd = 1.0,     //拡散反射係数
            ks = 0,     //鏡面反射係数
            ke = 0,     //環境反射係数
            n = 5,     //鏡面反射の強度係数
            s = 0.5;      //入射光の強さ
    double  cosa = -1 * dot(L, N);
    double  cosb = 2 * dot(L, N) * dot(N, V) - dot(L, V);
    double  C1 = s * kd * cosa + ke,
            C2 = s * ks * pow(cosb, n) * 255;
    struct color ret = {C1 * Cs.r + C2, C1 * Cs.g + C2, C1 * Cs.b + C2};
    return ret;
}

struct vector sphere_hit(struct vector v){
    struct vector P = {20, 0, 0};
    struct vector ret = {0, 0, 0};
    double r = 50;
    double d = (r * r - P.x * P.x)*(v.y * v.y + v.z * v.z) - (v.x * r) * (v.x * r); //判別式
    if(d >= 0){
        double t1 = (v.x * P.x + sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double t2 = (v.x * P.x - sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double l1 = sqrt(pow(v.x*t1, 2) + pow(v.y*t1, 2) + pow(v.z*t1, 2));
        double l2 = sqrt(pow(v.x*t2, 2) + pow(v.y*t2, 2) + pow(v.z*t2, 2));
        if(l2 < l1){
            ret.x = v.x * t2,
            ret.y = v.y * t2,
            ret.z = v.z * t2;
        }
        else if (l2 >= l1){
            ret.x = v.x * t1,
            ret.y = v.y * t1,
            ret.z = v.z * t1;
        }
    }
    return ret;
}

void hit_test(void){
    int i, j;
    struct color sc = {255, 0, 0};
    for(i = 0; i < WIDTH; i++){
        for(j = 0; j < HEIGHT; j++){
            struct vector v = {a, i - (WIDTH/2), j - (HEIGHT/2)};
            struct vector V = normalize(v);
            struct vector n = sphere_hit(V);
            struct vector N = normalize(n);
            struct vector l = {n.x - light.x, n.y - light.y, n.z - light.z};
            struct vector L = normalize(l);
            if(N.x != 0 && N.y != 0 && N.z != 0){
                sc = phong(N, L, V, sc);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
        }
    }
}

int main(void){
    struct color c1 = {30, 255, 0};
    struct color c2 = {255, 0, 0};
    img_clear();
    hit_test();
    img_write();
    /*
    int i;
    for(i = 0; i < 20; i++){
        img_clear();
        img_fillcircle(c1, 20+i*8, 100, 20);
        img_write();
    }
    for(i = 0; i < 20; i++){
        img_clear();
        img_fillcircle(c2, 180, 100+i*5, 20-i);
        img_write();
    }
    */
    return 0;
}
