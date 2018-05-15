<!-- マークダウンでブログ記事を書くテスト -->
<!-- 
Title:testPage
Descripton:サンプル記事
Data:2018/05/03
Category:雑記
 -->

##  ブログタイトル
May 5, 2018

### はじめに
この記事はブログをマークダウンのテストのために書かれています。  
全体の構成はぼんやりと決まっていますが、うまくできるとは限らないし、そもそもどれだけ時間がかかるかもわからない  
JavascriptとJSONを使えばいいということしかわからない素人でどうにかなるのか

### やりたいこと
目標としてはCMSとか使わずにできるだけ楽にブログの更新がしたい  
というわけで以下のような手順で進める  
1. 記事をマークダウンで書く
2. pythonか何かでマークダウンで書かれた記事をHTMLに変換する  
最初はとりあえず手書きでもよい
3. マークダウンを変換する際に記事のメタデータ（日付とかタイトルとか)をJSONファイルに出力する
4. JSONファイルをもとにJSが日付順とかカテゴリ分けしたページを作る（はず）

できるかわからないけどイメージとしてはこんな感じで

### マークダウン記法のテスト
引用
> もいもいももいもいももいもい  
> おほほいおほほいおほほい

文字色  
<font color="Red">エルフの森を燃やせ！！！！</font>

bold  
細い**太い**

itaric  
ノーマルあ*ああ斜体*

水平線
***

コードの挿入  
```c:test
int a = 0;
printf("HelloWorld!\n"); 
```

リンク  
[qiita](http://qiita.com/)

テスト `sudo apt-get install sl` みたいな

### 画像の挿入テスト
![meumeu](https://cloud.githubusercontent.com/assets/9606695/20932103/d33739ba-bc16-11e6-9921-f2f55ad4f69d.jpg "めうめう")

![circuit](https://cloud.githubusercontent.com/assets/9606695/20936868/a5c58c32-bc27-11e6-8b3a-1dad307b6d01.jpg )
<img src="https://cloud.githubusercontent.com/assets/9606695/20936868/a5c58c32-bc27-11e6-8b3a-1dad307b6d01.jpg" width=200>

### おわりに
早く研究に戻れ