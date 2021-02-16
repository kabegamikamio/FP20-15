/*
    コーディングのペースが遅い人は，書き写すことに集中し，無理してでも
    速く書きうつす．

    以下は毎回おなじ注意でくどいのですが，自分の脳を鍛えるために
    自分で自分を監視することがとても重要なので，改めて書いておきます．

    Twitterをやめろ
*/

//円を塗りつぶす関数
//テキストからの丸コピだから不要
void img_fillcircle(struct color c, double x, double y, double r){
    int imin = (int)(x-r-1), imax = (int)(x+r+1);
    int jmin = (int)(y-r-1), jmax = (int)(y+r+1);
    int i, j;
    for(j = jmin; j <= jmax; j++){
        for(i = imin; i <= imax; i++){
            if((x-i)*(x-i) + (y-j)*(y-j) <= r*r){
                img_putpixel(c, i, j);
            }
        }
    }
}

//ここに演習問題が入る