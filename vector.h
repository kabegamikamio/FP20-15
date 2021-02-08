//Vector definition
struct vector{
    double x, y, z;
};

double dot(struct vector a, struct vector b);                       //内積
struct vector product(struct vector a, struct vector b);            //外積
struct vector normlize(struct vector a);                            //正規化
struct vector rotation(struct vector a, double theta, double phi);  //回転
struct vector reflect(struct vector a, struct vector p, struct vector q);//反射、ベクトルpqからなる平面
