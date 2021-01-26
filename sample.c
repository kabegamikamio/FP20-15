//
// Created by izurina on 2021/01/26.
//

#include <stdio.h>
#include <math.h>
#include "img.h"

void img_fillrect(struct color c, double x, double y, double w, double h) {
    int imin = (int)(x - w/2), imax = (int)(x + w/2);
    int jmin = (int)(y - h/2), jmax = (int)(y + h/2);
    int i, j;
    for(j = jmin; j <= jmax; ++j) {
        for(i = imin; i <= imax; ++i) { img_putpixel(c, i, j); }
    }
}

void img_fillellipse(struct color c, double x, double y, double rx, double ry) {
    int imin = (int)(x - rx - 1), imax = (int)(x + rx + 1);
    int jmin = (int)(y - ry - 1), jmax = (int)(y + ry + 1);
    int i, j;
    for(j = jmin; j <= jmax; ++j) {
        for(i = imin; i <= imax; ++i) {
            if((x-i)*(x-i)/(rx*rx) + (y-j)*(y-j)/(ry*ry) <= 1.0) {
                img_putpixel(c, i, j);
            }
        }
    }
}

void img_fillrotellipse(struct color c, double x, double y, double rx, double ry,
                        double theta) {
    int d = (rx > ry) ? rx : ry;
    int imin = (int)(x - d - 1), imax = (int)(x + d + 1);
    int jmin = (int)(y - d - 1), jmax = (int)(y + d + 1);
    int i, j;
    for(j = jmin; j <= jmax; ++j) {
        for(i = imin; i <= imax; ++i) {
            double dx = i - x, dy = j - y;
            double px = (dx*cos(theta) - dy*sin(theta)) / rx;
            double py = (dx*sin(theta) + dy*cos(theta)) / ry;
            if(px*px + py*py <= 1.0) { img_putpixel(c, i, j); }
        }
    }
}

static double oprod(double a, double b, double c, double d) {
    return a*d - b*c;
}

static int isinside(double x, double y, int n, double ax[], double ay[]) {
    int i;
    for(i = 0; i < n; ++i) {
        if(oprod(ax[i+1]-ax[i], ay[i+1]-ay[i], x-ax[i], y-ay[i]) < 0) { return 0; }
    }
    return 1;
}

static double amax(int n, double a[]) {
    double m = a[0];
    int i;
    for(i = 0; i < n; ++i) { if(m < a[i]) { m = a[i]; } }
    return m;
}

static double amin(int n, double a[]) {
    double m = a[0];
    int i;
    for(i = 0; i < n; ++i) { if(m > a[i]) { m = a[i]; } }
    return m;
}

void img_fillconvex(struct color c, int n, double ax[], double ay[]) {
    int xmax = (int)(amax(n, ax)+1), xmin = (int)(amin(n, ax)-1);
    int ymax = (int)(amax(n, ay)+1), ymin = (int)(amin(n, ay)-1);
    int i, j;
    for(i = xmin; i <= xmax; ++i) {
        for(j = ymin; j <= ymax; ++j) {
            if(isinside(i, j, n, ax, ay)) { img_putpixel(c, i, j); }
        }
    }
}

void img_filltriangle(struct color c, double x0, double y0,
                      double x1, double y1, double x2, double y2) {
    double ax1[] = { x0, x1, x2, x0 }, ax2[] = { x0, x2, x1, x0 };
    double ay1[] = { y0, y1, y2, y0 }, ay2[] = { y0, y2, y1, y0 };
    img_fillconvex(c, 3, ax1, ay1);
    img_fillconvex(c, 3, ax2, ay2);
}

void img_fillline(struct color c, double x0, double y0,
                  double x1, double y1, double w) {
    double dx = y1-y0, dy = x0-x1, n = 0.5*w / sqrt(dx*dx + dy*dy);
    dx *= n; dy *= n;
    double ax[] = { x0-dx, x0+dx, x1+dx, x1-dx, x0-dx };
    double ay[] = { y0-dy, y0+dy, y1+dy, y1-dy, y0-dy };
    img_fillconvex(c, 4, ax, ay);
}
