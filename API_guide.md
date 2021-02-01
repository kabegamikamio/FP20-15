# レイトレ((実装予定))API

## ベクトル演算系
### ベクトル表記
ベクトルは(x, y, z)座標を用いて，struct vector構造体に格納される．
各座標はdouble型が指定されている．
以後簡単のためにstruct vector型はvector型として略記する．

### 内積 double dot(vector a, vector b)
2ベクトル a 及び b を与えるとその内積を実数値で返す関数．
返り値はdouble型である．

### 外積 vector product(vector a, vector b)
2ベクトル a 及び b を与えるとその外積をvector型で返す関数．
正規化が必要な場合は別途正規化関数を呼び出すこと．

### 正規化 vector normalize(vector a)
与えられたベクトル a を長さ1の方向ベクトルに変換(=正規化)する関数．
ベクトル a のノームをdouble型で計算し，各座標をノームで割っている．
誤差が出やすいかも．
