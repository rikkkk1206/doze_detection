## 心拍センサによる居眠り検出システム

このシステムは指先に取り付けた心拍センサによって心拍数を測定し、毎秒の心拍数から居眠りを検出するシステムです。

## デモ

睡眠中には安静時と比較して心拍数が約10%低下することがある研究で判明している[^睡眠に対する運動の効果-睡眠時の自律神経バランスに注目して-]。  
これを参考に、1秒間隔で心拍数を測定し続け、心拍数が安静時と比較して約10%低下したら居眠りと検出する。

![pbl-demo (1)](https://user-images.githubusercontent.com/86580511/144958001-ee1d00a9-2a3d-4eba-96c7-1323ad18c3c6.gif)

## 使用デバイス
* 心拍センサ (https://www.switch-science.com/catalog/1135/)

![c44725b2-4f3c-4dc9-9cf1-ff767c1b487c](https://user-images.githubusercontent.com/86580511/144838189-028562ee-138d-4aa9-a3f2-f77d684283e9.jpg)

* Arduino Uno

![100000001003862551_10204](https://user-images.githubusercontent.com/86580511/144838386-940df853-87e1-40fe-98c7-4012843f6327.jpg)

## 使用方法
1. Arduino IDEをインストール (https://www.arduino.cc/en/software)
2. PBL_heart_rate.inoをArduino IDEで開く
3. 心拍センサのスタートガイド (https://pulsesensor.com/pages/code-and-guide)
   を参考に、Arduino Unoと心拍センサを配線する
4. Arduino UnoをPCと接続しPBL_heart_rate.inoを書き込む
5. 指に心拍センサを取り付けて使用する

[^睡眠に対する運動の効果-睡眠時の自律神経バランスに注目して-]: 福場良之, 遠藤(山岡)雅子. 睡眠に対する運動の効果-睡眠時の自律神経バランスに注目して-. デサントスポーツ科学, 2011, Vol.32, p. 88-97,  
http://www.shinshu-u.ac.jp/faculty/textiles/db/seeds/descente32_11_fukuba.pdf
