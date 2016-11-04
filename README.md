# Proen
研究室課題がんばるぞい

## 課題6
OpenCVで目の検出

* ルートにAnacondaでインストールしたOpenCVのバージョンをおいて，カスケードのパスをそこに指定したら動いたっぽい  
* windows側のPATHは変更してない  
* opencvのインストールは [Windows Anaconda Python 3.5 への OpenCV 3.1 のインストール](http://nixeneko.hatenablog.com/entry/2016/01/20/012509)を参考にAnacondaにインストール

結果的にカスケードのパスが間違ってたっぽい  
Anaconda3経由で入れたカスケードだと精度が低かったため，OpenCV3.1.0をDLして使用
