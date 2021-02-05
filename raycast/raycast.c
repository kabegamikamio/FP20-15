/*
 * レイキャスティング法によるレンダリング
 * Ver.2015.02.08
 */
#include "img.h"
#include "vect.h"
#include "buf.h"
#include <math.h>

// 簡単化などのための仮定：
// ・座標系（x, y, z）＝（右，上，手前）
//     - スクリーンは xy 平面上（z = 0）
//     - 視点は +z 軸上
// ・3D 的なスクリーンのサイズ＝出力画像のサイズ
// ・3D 的な原点 ＝ スクリーンの中心
// ・方向ベクトルを常に正規化しておく


/* Material（材料）型構造体の定義 */
typedef struct {
	double diff[3];	// 拡散反射率 Kd の RGB 値
	double spec[3];	// 鏡面反射率 Ks の RGB 値
	int    high;	// ハイライトパラメータ n
} Material;

/* Sphere（球体）型構造体の定義 */
typedef struct {
	Vect   pos;	// 中心座標
	double rad;	// 半径
	int    mat;	// 材料番号
} Sphere;


/* 球体へのレイキャスティング
 * 入力：球体 s，視点 eye，視線 view
 * 参照返し：注視点 lookat，法線 normal
 * return：距離
*/
double RayCasting(Sphere s, Vect eye, Vect view, Vect *lookat, Vect *normal)
{
	double a, b, c;	// 二次方程式 a t^2 + b t + c = 0 の係数
	double d;	// 二次方程式の判別式
	double t;	// 二次方程式の解（視点-球体間の距離）

	a = 1.0;	// or  a = vLen2(view);
	b = 2.0*vDot(view, vSub(eye, s.pos));
	c = vLen2(vSub(eye, s.pos)) - s.rad*s.rad;
	d = b*b - 4.0*a*c;
	if (d < 0.0) return (-1.0);	// 交点がない→解がない
	if (b >= 0.0) return (-1.0);	// 球体が視線の逆方向にある→妥当な解がない
	if (c < 0.0) return (-1.0);	// 視点が球内にある→妥当な解がない

	t = (b + sqrt(d))/(-2.0*a);	// 2 つの解のうち小さい方
	if (lookat != NULL) *lookat = vAdd(eye, vScale(view, t));
	if (normal != NULL) *normal = vUnit(vSub(*lookat, s.pos));
	return (t);	// or  return (vLen(view)*t);
}


int main(int argc, char **argv)
{
// -------- ここから ------ 初期値を適宜変更可 ------ ここから --------
	Pix bg[] = { 64, 128, 192};	// 背景色

	Material mat[] = {	// 材料パラメータ
		{{0.40, 0.25, 0.10}, {0.80, 0.50, 0.20}, 4},  // mat[0]: gold
		{{0.45, 0.46, 0.49}, {0.90, 0.91, 0.98}, 4},  // mat[1]: silver
		{{0.36, 0.23, 0.10}, {0.72, 0.45, 0.20}, 4},  // mat[2]: copper
		{{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 20},	// mat[3]: plastic1
		{{0.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, 20},	// mat[4]: plastic2
		{{0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, 20},	// mat[5]: plastic3
	};

	Sphere   sph[] = {	// 球の位置，半径，材料
		{{- 60.0,  60.0, -60.0}, 50.0, 0},
		{{  20.0,  40.0, -30.0}, 50.0, 1},
		{{ 100.0,  20.0,   0.0}, 50.0, 2},
		{{-100.0, -20.0, -30.0}, 50.0, 3},
		{{- 20.0, -40.0,   0.0}, 50.0, 4},
		{{  60.0, -60.0,  30.0}, 50.0, 5},
	};
	int      n = 6;		// 球の個数

	int  w = 400, h = 300;	// スクリーン（画像）のサイズ
	int  ez = 500;		// 視点の z 座標
	Vect light = {1.0, 1.0, 1.0};	// 照明光の方向ベクトル Ｌ
	double amb = 0.5;	// 環境光強度 Ia
	double inc = 1.0;	// 照明光強度 Ip


// -------- ここまで -------------------------------- ここまで --------

	Img *img;	// 画像
	Buf *buf;	// デプスバッファ
	int x, y;	// スクリーン座標
	Pix val[3];	// 画素値

	double shade;	// 陰影
	double cosd;	// 拡散反射の角度因子 cosθ
	double coss;	// 鏡面反射の角度因子   cosγ
	double cosh;	// ハイライトの因子   cosγ^n
	Vect lookat;	// 注視点の位置ベクトル   Ｐ
	Vect normal;	// 法線の方向ベクトル     Ｎ
	Vect reflect;	// 鏡面反射の方向ベクトル Ｒ
	Vect view;	// 視線の方向ベクトル     Ｖ
	Vect eye;	// 視点の位置ベクトル     Ｅ

	double depth;	// 深度値
	double dist;	// 視点-物体間の距離
	Vect tmp;	// 作業用ベクトル変数
	int  i, m, c;	// 球番号，材料番号，色チャネル

	/* 画像の初期化 */
	img = NewImg(w, h);
	ClearImg(img, bg);		// 背景色でクリア

	/* デプスバッファの初期化 */
	buf = NewBuf(w, h);
	ClearBuf(buf, 1.0/0.0);		// 深度 ∞ でクリア

	/* シェーディング用ベクトルの初期化 */
	eye = vInit(0.0, 0.0, ez);	// 視点ベクトル
	light = vUnit(light);		// 照明ベクトル

	/* レンダリング */
	for (y = 0; y < h; y++)
	for (x = 0; x < w; x++) {	// ラスタスキャン
		tmp = vInit(x - w/2.0, h/2.0 - y, 0.0);	// 画素の位置ベクトル
		view = vUnit(vSub(tmp, eye));		// 視線ベクトル

		GetDepth(buf, x, y, &depth);		// バッファの深度値
		for (i = 0; i < n; i++) {
			/* レイキャスティング */
			dist = RayCasting(sph[i], eye, view, &lookat, &normal);
			if (dist < 0.0) continue;	// 交差なし→次の球へ

			/* デプスバッファリング */
			if (dist >= depth) continue;	// 隠面→次の球へ

			depth = dist;
			SetDepth(buf, x, y, depth);	// 深度の更新

			/* 拡散反射 */
			cosd = vDot(light, normal);
			if (cosd < 0.0) cosd = 0.0;


			/* 鏡面反射 */
			coss = 0.0;	// とりあえず無効化
			// ... 各自で実装せよ!!

			m = sph[i].mat;			// この球の材料番号
			cosh = pow(coss, (double)mat[m].high);

			/* シャドーイング */
			// .... 余裕がある者は実装せよ!!

			/* 描画 */
			for (c = 0; c < 3; c++) {
				shade = mat[m].diff[c]*amb		// 環境
					+ mat[m].diff[c]*inc*cosd	// 拡散
					+ mat[m].spec[c]*inc*cosh;	// 鏡面
				if (shade > 1.0) shade = 1.0;	// 1.0以内に

				val[c] = (Pix)(255*shade);	// 画素値（255以内）
			}
			SetPixel(img, x, y, val);
		}
	}

	PreviewImg(img, "RayCasting");	// 画像を表示

	FreeImg(img);
	FreeBuf(buf);
	return (0);
}

