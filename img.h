//This file is defining API
#define WIDTH 1920
#define HEIGHT 1080
#define DISTANCE 50 //視点から画素面までの距離

struct color{
    unsigned char r, g, b;
};

struct vector{
    double x, y, z;
};

void img_clear(void);
void img_write(void);
void img_putpixel(struct color c, int x, int y);
void img_fillcircle(struct color c, double x, double y, double r);
void img_fillellipse(struct color c, double x, double y, double rx, double ry);
void img_fillrotellipse(struct color c, double x, double y, double rx, double ry, double theta);
double v_dot(struct vector a, struct vector b);
struct vector product(struct vector a, struct vector b);
struct vector normalize(struct vector a);
struct vector rotation(struct vector a, double theta, double phi);
struct vector reflect(struct vector a, struct vector n);
struct vector cross_point(struct vector p, struct vector v, struct vector q, struct vector n);
struct vector sphere_hit(struct vector v);
void hit_test(void);