/*
    コーディングのペースが遅い人は，書き写すことに集中し，無理してでも
    速く書きうつす．

    以下は毎回おなじ注意でくどいのですが，自分の脳を鍛えるために
    自分で自分を監視することがとても重要なので，改めて書いておきます．

    Twitterをやめろ
*/

#include <stdlib.h>
#include <math.h>
#include "img.h"

int a = -499;     //視点から画素までの距離
struct vector light = {-1000, 2000, 0};     //光源の座標

//球の中心座標，半径，
struct vector P1 = {700, 100, 0};  double r1 = 75;   struct color c1 = {255, 0, 0};
struct vector P2 = {700, -100, 0};  double r2 = 75;   struct color c2 = {0, 0, 255};

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

//球と視線ベクトルの交点を求める関数
//視線ベクトルvは単位ベクトルとして与える
struct vector sphere_cross(double r, struct vector P, struct vector v){
    struct vector intersect = {0, 0, 0};  //返す座標は(0, 0, 0)で初期化，(0, 0, 0)は交点がない時に使う
    double Pnorm2 = pow(P.x, 2) + pow(P.y, 2) + pow(P.z, 2);    //原点から点Pまでの距離の2乗
    double d = pow(P.x * v.x + P.y * v.y + P.z * v.z, 2) - (Pnorm2 - pow(r, 2)); //判別式

    //判別式での判定
    //double型の誤差も踏まえてd = 0ではなくd >= 0としている
    if(d >= 0){
        double t1 = (v.x * P.x + v.y * P.y + v.z * P.z + sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double t2 = (v.x * P.x + v.y * P.y + v.z * P.z - sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double l1 = sqrt(pow(v.x*t1, 2) + pow(v.y*t1, 2) + pow(v.z*t1, 2));
        double l2 = sqrt(pow(v.x*t2, 2) + pow(v.y*t2, 2) + pow(v.z*t2, 2));

        //視点からの距離が近い方の交点だけreturnする
        if(l2 < l1){
            intersect.x = v.x * t2,
            intersect.y = v.y * t2,
            intersect.z = v.z * t2;
        }else if (l2 >= l1){
            intersect.x = v.x * t1,
            intersect.y = v.y * t1,
            intersect.z = v.z * t1;
        }
    }
    return intersect;
}

//光学計算を行わせる関数
//実際の計算は下請け関数に任せて，呼び出しはこの関数に集約する
void sphere_hit(double r, struct vector P, struct color c){
    int i, j;
    for(i = 0; i < WIDTH; i++){
        for(j = 0; j < HEIGHT; j++){

            //ベクトルVは視線の方向ベクトル
            struct vector v = {a, i - (WIDTH/2), j - (HEIGHT/2)};
            struct vector V = normalize(v);

            //ベクトルNは平面の法線ベクトル
            //N = (視線との交点) - (球心の位置ベクトル)
            struct vector n0 = sphere_cross(r, P, V);
            struct vector n = {n0.x - P.x, n0.y - P.y, n0.z - P.z};
            struct vector N = normalize(n);

            //ベクトルLは光線の単位ベクトル
            //L = (視線との交点) - (光源の位置ベクトル)
            struct vector l = {n0.x - light.x, n0.y - light.y, n0.z - light.z};
            struct vector L = normalize(l);

            //視線と球の交点が(0, 0, 0)の場合は交点がないことを意味するので除外
            if(n0.x != 0 || n0.y != 0 || n0.z != 0){
                struct color sc = phong(N, L, V, c);            //フォンモデルで計算
                //sc = distance_ray(sc, light, n);  //自作モデルで計算
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;  //バッファにRGB値を格納
            }
        }
    }
}

void box(void){
    int i, j;
    struct color gray = {100, 100, 100};
    struct color white = {255, 225, 225};
    struct color wall_color = {50, 75, 155};
    struct color green = {0, 255, 0};
    struct color ceiling_color = {51, 50, 101};
    struct vector origin = {0, 0, 0};
    struct vector nx = {1, 0, 0}, ny = {0, 1, 0}, nz = {0, 0, 1};   //各軸に平行な単位ベクトル．面の法線ベクトルとして使う
    for(i = 0; i < WIDTH; i++){
        for(j = 0; j < HEIGHT; j++){
            struct vector v = {-a, i - WIDTH/2, j - HEIGHT/2};
            struct vector V = normalize(v);
            double t = 150 / V.z;
            if(fabs(V.x * t - 500) <= 500 && fabs(V.y * t) <= 300){
                struct vector l = {V.x * t - light.x, V.y * t - light.y, 150 - light.z};
                struct vector L = normalize(l);
                struct color sc = phong(nz, L, V, gray);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
            t = -150 / V.z;
            if(fabs(V.x * t - 500) <= 500 && fabs(V.y * t) <= 300){
                struct vector l = {V.x * t - light.x, V.y * t - light.y, 150 - light.z};
                struct vector L = normalize(l);
                struct color sc = phong(nz, L, V, ceiling_color);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
            double u = 1000 / V.x;
            if(fabs(V.z * u) <= 150 && fabs(V.y * u) <= 300){
                struct vector l = {1000 - light.x, V.y * u - light.y, V.z*u - light.z};
                struct vector L = normalize(l);
                struct color sc = phong(nz, L, V, wall_color);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
            double k = 300 / V.y;
            if(fabs(V.z * k) <= 150 && fabs(V.x * k - 500) <= 500){
                struct vector l = {V.x*k - light.x, 300 - light.y, V.z*k - light.z};
                struct vector L = normalize(l);
                struct color sc = phong(nz, L, V, wall_color);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
            k = -300 / V.y;
            if(fabs(V.z * k) <= 150 && fabs(V.x * k - 500) <= 500){
                struct vector l = {V.x*k - light.x, 300 - light.y, V.z*k - light.z};
                struct vector L = normalize(l);
                struct color sc = phong(nz, L, V, wall_color);
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;
            }
        }
    }
}

void hit_test(void){
    sphere_hit(r2, P2, c2);
    sphere_hit(r1, P1, c1);
}

//ここに演習問題が入る