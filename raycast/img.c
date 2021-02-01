#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"

#define PUBLIC
#define PRIVATE	static

#define BUFLEN	256

PRIVATE void fatal(char *msg) { fprintf(stderr, "%s\n", msg); exit(1); }
PRIVATE void debug(char *msg) { fprintf(stderr, "%s\n", msg); }

PRIVATE int mkCmd(char *buf, int n, const char *cmd, const char *file)
{
	if (strlen(cmd)+strlen(file) > n) return (1);
	sprintf(buf, cmd, file);
	return (0);
}

PRIVATE int GetImgSize(const char *file, int *w, int *h)
{
	FILE *pp;
	char buf[BUFLEN];
	int  n;

	if (mkCmd(buf, BUFLEN, IDENTIFY_CMD, file)) fatal("GetImgSize(): filename too long.");
	if ((pp = popen(buf, "r")) == NULL) fatal("GetImgSize(): couldn't identify.");
	n = fscanf(pp, "%d %d", w, h);
	pclose(pp);
	if (n != 2) fatal("GetImgSize(): couldn't get image size.");
	return (0);
}

PRIVATE FILE *OpenImgFile(const char *file)
{
	char buf[BUFLEN];

	if (mkCmd(buf, BUFLEN, CONVERT_CMD, file)) fatal("OpenImgFIle(): filename too long.");
	return (popen(buf, "r"));
}

PRIVATE void CloseImgFile(FILE *fp)
{
	pclose(fp);
}

// 画像を生成
PUBLIC Img *NewImg(int w, int h)
{
	Img *img;

	img = (Img *)malloc(sizeof(Img));
	if (img == NULL) goto ERROR;

	img->w = w;
	img->h = h;
	img->c = CHANNEL;
	img->d = DEPTH;

	img->pixel = (Pix *)calloc(w*h*4, sizeof(Pix));
	if (img->pixel == NULL) goto ERROR;

	return (img);

ERROR:
	FreeImg(img);
	return (NULL);
}

// 画像を破棄
PUBLIC void FreeImg(Img *img)
{
	if (img != NULL) free(img->pixel);
	free(img);
}

// 任意形式を RGBA 形式へ変換し入力
PUBLIC Img *ReadImg(const char *file)
{
	FILE *fp;
	Img *img;
	int  w, h;
	int  x, y, c;
	int  tmp;
	Pix  *p;

	GetImgSize(file, &w, &h);
	if ((img = NewImg(w, h)) == NULL) goto ERROR;
	if ((fp = OpenImgFile(file)) == NULL) goto ERROR;

	p = img->pixel;
	for (y = 0; y < h; y++)
	for (x = 0; x < w; x++)
	for (c = 0; c < CHANNEL; c++)
	{
		if ((tmp = fgetc(fp)) == EOF) goto ERROR;
		*p = (Pix)tmp;
		p++;
	}
	CloseImgFile(fp);
	return (img);

ERROR:
	CloseImgFile(fp);
	return (NULL);
}

// PPM 形式で出力（A 成分を無視）
PUBLIC int WriteImg(FILE *fp, const Img *img)
{
	int  x, y, c;
	Pix  *p;

	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", img->w, img->h);
	fprintf(fp, "%d\n", MAXVAL);
	p = img->pixel;
	for (y = 0; y < img->h; y++)
	for (x = 0; x < img->w; x++)
	{
		for (c = 0; c < 3; c++) {
			fputc(*p, fp);
			p++;
		}
		p++;
	}
	return (0);
}

PRIVATE int chkRange(const Img *img, int x, int y)
{
	if (x < 0) return (1);
	if (x >= img->w) return (1);
	if (y < 0) return (1);
	if (y >= img->h) return (1);
	return (0);
}

// 画素値配列をセット
PUBLIC int SetPixel(Img *img, int x, int y, const Pix *val)
{
	int  c;
	Pix  *p;

	if (chkRange(img, x, y)) return (1);

	p = img->pixel + (y*img->w + x)*4;
	for (c = 0; c < img->c; c++) {
		*p = *val;
		val++;
		p++;
	}
	return (0);
}

// 画素値配列をゲット
PUBLIC int GetPixel(const Img *img, int x, int y, Pix *val)
{
	int  c;
	Pix  *p;

	if (chkRange(img, x, y)) return (1);

	p = img->pixel + (y*img->w + x)*4;
	for (c = 0; c < img->c; c++) {
		*val = p[c];
		val++;
		p++;
	}
	return (0);
}

// 画像をクリア
PUBLIC int ClearImg(Img *img, const Pix *val)
{
	Pix  *p;
	int  x, y, c;

	p = img->pixel;
	for (y = 0; y < img->h; y++)
	for (x = 0; x < img->w; x++)
	for (c = 0; c < 4; c++)
	{
		*p = val[c];
		p++;
	}
	return (0);
}

// 画像を表示
PUBLIC int PreviewImg(const Img *img, const char *title)
{
	FILE *pp;
	char buf[BUFLEN];

	if (mkCmd(buf, BUFLEN, DISPLAY_CMD, title)) return (1);
	if ((pp = popen(buf, "w")) == NULL) return (1);
	WriteImg(pp, img);
	pclose(pp);
	return (0);
}

#ifdef __cplusplus
}
#endif

