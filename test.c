#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vector{
    double x, y, z;
};

//球と視線ベクトルの交点を求める関数
//視線ベクトルvは単位ベクトルとして与える
struct vector sphere_hit(struct vector v){
    struct vector P = {100, 0, 0};
    struct vector ret = {0, 0, 0};
    double r = 10;
    double Pnorm2 = pow(P.x, 2) + pow(P.y, 2) + pow(P.z, 2);    //原点から点Pまでの距離の2乗
    double d = pow(P.x * v.x + P.y * v.y + P.z * v.z, 2) - (Pnorm2 - pow(r, 2)); //判別式
    if(d >= 0){
        double t1 = (v.x * P.x + v.y * P.y + v.z * P.z + sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
        double t2 = (v.x * P.x + v.y * P.y + v.z * P.z - sqrt(d)) / (v.x*v.x + v.y*v.y + v.z*v.z);
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

int main(){
    struct vector a = {1, 0, 0};
    struct vector b = {8, 4, 2};
    struct vector n = {0, 0, -1};
    double theta = 3.141592 / 3;
    struct vector ret = sphere_hit(a);
    printf("%g %g %g", ret.x, ret.y, ret.z);
    return 0;
}