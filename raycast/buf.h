#ifndef BUF_H
#define BUF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int w, h;	// 幅，高さ
	double *pixel;	// 値
} Buf;		// 深度バッファ

extern Buf *NewBuf(int w, int h);	// 深度バッファを生成
extern void FreeBuf(Buf *buf);		// 深度バッファを破棄
extern int SetDepth(Buf *buf, int x, int y, double val);	// 深度値をセット
extern int GetDepth(Buf *buf, int x, int y, double *val);	// 深度値をゲット
extern int ClearBuf(Buf *buf, double val);	// 深度バッファをクリア

#ifdef __cplusplus
}
#endif

#endif

