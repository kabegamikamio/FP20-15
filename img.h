//This file is defining API
#define WIDTH 400
#define HEIGHT 300
#define DISTANCE 50 //視点から画素面までの距離

struct color{
    unsigned char r, g, b;
};

struct int_color{
    int r, g, b;
};

struct vector{
    double x, y, z;
};

extern int a;
extern struct vector light;
unsigned char buf[HEIGHT][WIDTH][3];

struct vector P1;   double r1;  struct color c1;
struct vector P2;   double r2;  struct color c2;

void img_clear(void);
void img_write(void);
void img_putpixel(struct color c, int x, int y);
void img_fillcircle(struct color c, double x, double y, double r);
void img_fillellipse(struct color c, double x, double y, double rx, double ry);
void img_fillrotellipse(struct color c, double x, double y, double rx, double ry, double theta);
double dot(struct vector a, struct vector b);
struct vector product(struct vector a, struct vector b);
struct vector normalize(struct vector a);
struct vector rotation(struct vector a, double theta, double phi);
struct vector reflect(struct vector a, struct vector n);
struct vector cross_point(struct vector p, struct vector v, struct vector q, struct vector n);
struct vector sphere_cross(double r, struct vector P, struct vector v);
void sphere_hit(double r, struct vector P, struct color c);
void hit_test(void);
struct color phong(struct vector N, struct vector L, struct vector V, struct color Cs);
struct vector decmov(struct vector S, struct vector G, float length, int frame);
struct vector accmov(struct vector S, struct vector G, float length, int frame);
struct vector kurukuru(struct vector S, double theta ,double phi, double x, double y, double z, float length, int frame);
struct vector deckuru(struct vector S, double theta ,double phi, double x, double y, double z, float length, int frame);
struct vector acckuru(struct vector S, double theta ,double phi, double x, double y, double z, float length, int frame);
