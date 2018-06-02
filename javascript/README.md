# lerning javascript

## HP制作について

### iframeではめ込み
ひとまずiframeを使ってブログを作る予定  
iframeで呼び出すところはできた  
実際に`blog.html`に記述してある([レスポンシブの参考](http://chogenba.com/8277/)、[背景透過の参考](http://lab.agr.hokudai.ac.jp/useful/CSS/h3_iframBackground.htm))  
sassの追加したりしたので使いまわせるはず  
子のhtmlは`testpost.html`を参考に、多分もっと最適化できる

### JSONの呼び出し
非同期処理につまずいて、最終的に`$.ajax`使った([jQueryのajax()を利用すると返り値をとりたいときのやり方](https://qiita.com/katsukii/items/bd64efcf4f070d77c028))  
`.done`内部で`console.log`がうまく使えなかったり、`return`でほしい値じゃなくてparseされる前の生データが帰ってきたりとよくわからないとこがあった（そういう仕様なのかも）
