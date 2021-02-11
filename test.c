#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vector{
    double x, y, z;
};

double v_dot(struct vector a, struct vector b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vector normalize(struct vector a){
    double norm = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
    a.x = a.x / norm,
    a.y = a.y / norm,
    a.z = a.z / norm;
    return a;
}

struct vector reflect(struct vector a, struct vector n){
    struct vector N = normalize(n);
    double mag;
        mag = v_dot(a,N);
    struct vector reflect;
        reflect.x = a.x + 2 * mag * N.x;
        reflect.y = a.y + 2 * mag * N.y;
        reflect.z = a.z + 2 * mag * N.z;
    return reflect;
}

int main(){
    struct vector a = {1, 2, 3};
    struct vector b = {8, 4, 2};
    struct vector n = {0, 0, -1};
    double theta = 3.141592 / 3;
    struct vector ret = reflect(a, n);
    printf("%g %g %g", ret.x, ret.y, ret.z);
    return 0;
}