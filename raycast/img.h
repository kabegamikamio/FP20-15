// 画像処理 API
// 要 ImageMagick

#ifndef IMG_H
#define IMG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct {
	int w, h;	// 幅，高さ
	int c, d;	// チャネル数，デプスビット数
	unsigned char *pixel;
} Img;

typedef unsigned char Pix;

// ImageMagick によるお手軽な任意形式画像の入力
#define IDENTIFY_CMD	"identify -format '%%w %%h' %s"
#define CONVERT_CMD	"convert %s -depth 8 rgba:-"
#define DISPLAY_CMD	"display -title '%s' ppm:-"
//#define DISPLAY_CMD	"~/bin/nodisplay.bash"		// display の代用

// 画素値内部形式
#define CHANNEL	4	// チャンネル数：3 or 4
#define DEPTH	8	// ビット数
#define MAXVAL	255	// 最大値

extern Img *NewImg(int w, int h);	// 画像を生成
extern void FreeImg(Img *img);		// 画像を破棄
extern Img *ReadImg(const char *file);		// 任意形式画像を入力
extern int WriteImg(FILE *fp, const Img *img);	// PPM 形式画像を出力
extern int SetPixel(Img *img, int x, int y, const Pix *val);	// 画素値配列をセット
extern int GetPixel(const Img *img, int x, int y, Pix *val);	// 画素値配列をゲット
extern int ClearImg(Img *img, const Pix *val);	// 画像をクリア
extern int PreviewImg(const Img *img, const char *title);	// 画像を表示

#ifdef __cplusplus
}
#endif

#endif

