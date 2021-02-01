// �������� API
// �� ImageMagick

#ifndef IMG_H
#define IMG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
	int w, h;	// �����⤵
	int c, d;	// ����ͥ�����ǥץ��ӥåȿ�
	unsigned char *pixel;
} Img;

typedef unsigned char Pix;

// ImageMagick �ˤ�뤪��ڤ�Ǥ�շ�������������
#define IDENTIFY_CMD	"identify -format '%%w %%h' %s"
#define CONVERT_CMD	"convert %s -depth 8 rgba:-"
#define DISPLAY_CMD	"display -title '%s' ppm:-"
//#define DISPLAY_CMD	"~/bin/nodisplay.bash"		// display ������

// ��������������
#define CHANNEL	4	// �����ͥ����3 or 4
#define DEPTH	8	// �ӥåȿ�
#define MAXVAL	255	// ������

extern Img *NewImg(int w, int h);	// ����������
extern void FreeImg(Img *img);		// �������˴�
extern Img *ReadImg(const char *file);		// Ǥ�շ�������������
extern int WriteImg(FILE *fp, const Img *img);	// PPM �������������
extern int SetPixel(Img *img, int x, int y, const Pix *val);	// ����������򥻥å�
extern int GetPixel(const Img *img, int x, int y, Pix *val);	// ����������򥲥å�
extern int ClearImg(Img *img, const Pix *val);	// �����򥯥ꥢ
extern int PreviewImg(const Img *img, const char *title);	// ������ɽ��

#ifdef __cplusplus
}
#endif

#endif

