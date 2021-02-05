#include <stdio.h>
#include <stdlib.h>
#include "buf.h"

#define PUBLIC
#define PRIVATE	static

// 深度バッファを生成
PUBLIC Buf *NewBuf(int w, int h)
{
	Buf *buf;

	buf = (Buf *)malloc(sizeof(Buf));
	if (buf == NULL) goto ERROR;

	buf->w = w;
	buf->h = h;

	buf->pixel = (double *)calloc(w*h, sizeof(double));
	if (buf->pixel == NULL) goto ERROR;

	return (buf);

ERROR:
	FreeBuf(buf);
	return (NULL);
}

// 深度バッファを破棄
PUBLIC void FreeBuf(Buf *buf)
{
	if (buf != NULL) free(buf->pixel);
	free(buf);
}

PRIVATE int bufChkRange(Buf *buf, int x, int y)
{
	if (x < 0) return (1);
	if (x >= buf->w) return (1);
	if (y < 0) return (1);
	if (y >= buf->h) return (1);
	return (0);
}

// 深度値をセット
PUBLIC int SetDepth(Buf *buf, int x, int y, double val)
{
	if (bufChkRange(buf, x, y)) return (1);

	*(buf->pixel + y*buf->w + x) = val;
	return (0);
}

// 深度値をゲット
PUBLIC int GetDepth(Buf *buf, int x, int y, double *val)
{
	if (val == NULL) return (1);
	if (bufChkRange(buf, x, y)) return (1);

	*val = *(buf->pixel + y*buf->w + x);
	return (0);
}

// 深度バッファをクリア
PUBLIC int ClearBuf(Buf *buf, double val)
{
	double *p;
	int x, y;

	p = buf->pixel;
	for (y = 0; y < buf->h; y++)
	for (x = 0; x < buf->w; x++) {
		*p = val;
		p++;
	}
	return (0);
}

