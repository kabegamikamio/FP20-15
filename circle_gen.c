#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img.h"

static int a = -499;
static unsigned char buf[HEIGHT][WIDTH][3];
static int filecnt = 0;
static char fname[100];
struct vector light = {500, 380, 500};

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

//円を塗りつぶす関数
//テキストからの丸コピだから不要
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

//int_colorをcolorに変換する関数
//ついでに0 ~ 255の間にRGBを直す
struct color color_range(struct int_color c0){
    int c[3] = {c0.r, c0.g, c0.b};
    int i;
    for(i = 0; i < 3; i++){
        if(c[i] < 0){c[i] = 0;}
        else if(c[i] > 255){c[i] = 255;}
    }
    struct color ret = {c[0], c[1], c[2]};
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
    double  kd = 1,     //拡散反射係数
            ks = 0.5,     //鏡面反射係数
            ke = 0.2,     //環境反射係数
            n = 10,     //鏡面反射の強度係数
            s = 1;      //入射光の強さ
    double  cosa = -1 * dot(L, N);
    double  cosb = 2 * dot(L, N) * dot(N, V) - dot(L, V);
    double  C1 = s * kd * cosa + ke,
            C2 = s * ks * pow(cosb, n) * 255;
    struct int_color C = {C1 * Cs.r + C2, C1 * Cs.g + C2, C1 * Cs.b + C2};
    struct color ret = color_range(C);
    return ret;
}

//球と視線ベクトルの交点を求める関数
//視線ベクトルvは単位ベクトルとして与える
struct vector sphere_hit(double r, struct vector P, struct vector v){
    struct vector ret = {0, 0, 0};  //返す座標は(0, 0, 0)で初期化，(0, 0, 0)は交点がない時に使う
    double r = 75;
    double Pnorm2 = pow(P.x, 2) + pow(P.y, 2) + pow(P.z, 2);    //原点から点Pまでの距離の2乗
    double d = pow(P.x * v.x + P.y * v.y + P.z * v.z, 2) - (Pnorm2 - pow(r, 2)); //判別式

    //判別式での判定
    //d = 0だと交点が見つからないからどこかおかしいのかもしれない…
    if(d >= 0){
        double t1 = (v.x * P.x + v.y * P.y + v.z * P.z + sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double t2 = (v.x * P.x + v.y * P.y + v.z * P.z - sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double l1 = sqrt(pow(v.x*t1, 2) + pow(v.y*t1, 2) + pow(v.z*t1, 2));
        double l2 = sqrt(pow(v.x*t2, 2) + pow(v.y*t2, 2) + pow(v.z*t2, 2));

        //視点からの距離が近い方の交点だけreturnする
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

//光学計算を行わせる関数
//実際の計算は下請け関数に任せて，呼び出しはこの関数に集約する
void hit_test(void){
    int i, j;
    struct color sc0 = {222, 0, 0};
    struct vector P = {1000, 0, 0};
    for(i = 0; i < WIDTH; i++){
        for(j = 0; j < HEIGHT; j++){

            //ベクトルVは視線の方向ベクトル
            struct vector v = {a, i - (WIDTH/2), j - (HEIGHT/2)};
            struct vector V = normalize(v);
        
            //ベクトルNは平面の法線ベクトル
            //N = (視線との交点) - (球心の位置ベクトル)
            struct vector n0 = sphere_hit(V);
            struct vector n = {n0.x - P.x, n0.y - P.y, n0.z - P.z};
            struct vector N = normalize(n);

            //ベクトルLは光線の単位ベクトル
            //L = (視線との交点) - (光源の位置ベクトル)
            struct vector l = {n0.x - light.x, n0.y - light.y, n0.z - light.z};
            struct vector L = normalize(l);

            //視線と球の交点が(0, 0, 0)の場合は交点がないことを意味するので除外
            if(n0.x != 0 || n0.y != 0 || n0.z != 0){
                struct color sc = phong(N, L, V, sc0);            //フォンモデルで計算
                //sc = distance_ray(sc, light, n);  //自作モデルで計算
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;  //バッファにRGB値を格納
            }
        }
    }
}

int main(void){
    int i;
    for(i = 0; i < 20; i++){
        img_clear();
        hit_test();
        img_write();
        light.y -= 20 * i;
    }
    for(i = 0; i < 20; i++){
        img_clear();
        hit_test();
        img_write();
        light.y += 20 * i;
    }
    for(i = 0; i < 20; i++){
        img_clear();
        hit_test();
        img_write();
        P.z -= 10 * i;
        P.x += 20 * i;
    }
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
