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
struct vector P1 = {700, 0, 200};  double r1 = 75;   struct color c1 = {255, 0, 0};
struct vector P2 = {700, 0, 200};  double r2 = 75;   struct color c2 = {0, 0, 255};

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
struct vector sphere_hit(double r, struct vector P, struct vector v){
    struct vector ret = {0, 0, 0};  //返す座標は(0, 0, 0)で初期化，(0, 0, 0)は交点がない時に使う
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
    for(i = 0; i < WIDTH; i++){
        for(j = 0; j < HEIGHT; j++){

            //ベクトルVは視線の方向ベクトル
            struct vector v1 = {a, i - (WIDTH/2), j - (HEIGHT/2)};
            struct vector V1 = normalize(v1);
        
            //ベクトルNは平面の法線ベクトル
            //N = (視線との交点) - (球心の位置ベクトル)
            struct vector n01 = sphere_hit(75, P1, V1);
            struct vector n1 = {n01.x - P1.x, n01.y - P1.y, n01.z - P1.z};
            struct vector N1 = normalize(n1);

            //ベクトルLは光線の単位ベクトル
            //L = (視線との交点) - (光源の位置ベクトル)
            struct vector l1 = {n01.x - light.x, n01.y - light.y, n01.z - light.z};
            struct vector L1 = normalize(l1);

            //視線と球の交点が(0, 0, 0)の場合は交点がないことを意味するので除外
            if(n01.x != 0 || n01.y != 0 || n01.z != 0){
                struct color sc = phong(N1, L1, V1, c1);          //フォンモデルで計算
                buf[j][i][0] = sc.r, buf[j][i][1] = sc.g, buf[j][i][2] = sc.b;  //バッファにRGB値を格納
            }
        }
    }
}

//ここに演習問題が入る