//安静時間の平均を全体の平均で求める・画像処理とのマージを終えてる

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

#define Time 180   //安静時の平均BPMを計測する時間(s)
#define Avenum 30   //移動平均を計算する範囲(s)
int bpm = 0;   //ユーザの現在のBPMを受け取る
int sum_bpm = 0, ave_bpm = 0;   //安静時の平均BPMを計算するための変数
int cnt = 0;   //実行回数を格納する
int data[Avenum];   //移動平均を計算する範囲の値を格納する
int sum_data = 0, ave_data = 0;   //移動平均を計算するための変数

const int OUTPUT_TYPE = SERIAL_PLOTTER;

//
int a[Time];
int b[Time/2];
int b1 = (Time/2)/2, b2 = Time-((Time/2)/2);
int p, tmp;
int i, j = 0;


void quick(int a[Time], int l, int u)//クイックソート、大きい値・小さい値を弾くために四分位数を求めるのに使う
{

  if(l >= u)
  {
    return;
  }
  if(a[l] > a[l+1])
  {
    p = a[l];
  }else{
    p = a[l+1];
  }

  i = l;
  j = u;
  while(i < j)
  {
    while(a[i] < p && i < j)
    {
      i++;
    }
    while(p <= a[j] && i < j)
    {
      j--;
    }
    if(a[i] > a[j])
    {
      tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
    }
  }
  if(l > i-1)
  {
    i++;
  }
  quick(a, l, i-1);
  quick(a, i, u);
}


const int PULSE_INPUT = A0;
const int PULSE_BLINK = 13;
const int PULSE_FADE = 5;
const int THRESHOLD = 550;

const int led = 12;   //移動平均が安静時の平均BPMを下回る際にPin12のLEDを点灯させる

const int p_led = 11;   //移動平均が安静時の平均BPMを下回る際にPin12のLEDを点灯させる

PulseSensorPlayground pulseSensor;

void setup() {

  Serial.begin(115200);

  pinMode(led, OUTPUT);   //Pin12の準備
  pinMode(p_led, OUTPUT);

  Serial.println("5秒間待機");


  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  if (!pulseSensor.begin()) {
  
    for(;;) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

void loop() {
 delay(1000);   //1秒毎に繰り返す
 bpm = pulseSensor.getBeatsPerMinute();   //BPMの値を受け取る

 if(bpm != 0)   //異常値を減らすために0は無視する
 {

  //安静時の平均BPM
  if(cnt == (Time+6))   //Time秒(安静時間)経ったら
  {
   //ave_bpm = sum_bpm / cnt;   //平均BPMを算出
   //
   Serial.print("a[]:");
   for(i = 0; i < Time; i++)  //確認用の出力部
   {
    Serial.print(a[i]);
    Serial.print(", ");
   }
   Serial.println("");
   Serial.print("sort:");
   quick(a, 0, Time-1);   //昇順にソート
   for(i = 0; i < Time; i++)  //確認用の出力部
   {
    Serial.print(a[i]);
    Serial.print(", ");
   }
   Serial.println("");
   Serial.print("sibunni:");
   j = 0;
   for(i = b1; i < b2; i++)   //a[]の四分位範囲をb[]に移す
   {
    b[j] = a[i];
    Serial.print(b[i]);     
    Serial.print(", ");     
    j++;
   }
   Serial.println("");      
   for(i = 0; i < (Time/2); i++)  //四分位範囲を合計する
    sum_bpm = sum_bpm + b[i];

   ave_bpm = sum_bpm / (Time/2);   //平均BPMを算出
   //
   Serial.print(Time);              //確認用の出力部
   Serial.print("秒間の平均BPM：");
   Serial.println(ave_bpm); 
  }
  //毎回(1秒ごと)のBPMを表示
  if(cnt > 5)   //最初の5秒前後は安定しないため、それ以降のとき
  {
   Serial.print("BPM：");   
   Serial.println(bpm);     
   if(cnt < Time+6)  //安静時間中の毎秒BPMを配列に格納
   {
    a[cnt-6] = bpm;
    /*if(cnt < Time)
      sum_bpm = sum_bpm + bpm;   //安静時間を過ぎたあとは合計値を残す必要がない
      */
   }
  }

  //移動平均を計算する範囲の値を準備する
  if(cnt >= (Time+6)-(Avenum-1))   //安静時間が終わるAvenum(移動平均の範囲)秒前から値を格納し始める
  {
   for(int i = Avenum-1; i > 0; i--)   //data[]に格納されているBPMをずらしていく
   {
    data[i] = data[i-1];
    Serial.print(data[i]);   
    Serial.print(",");       
   }
   data[0] = bpm;   //今回のBPMを格納する
   Serial.println(data[0]);  
  }

  //安静時間後の動作
  if(cnt > Time+6)   //安静時間を過ぎたら
  {
   sum_data = 0;   //初期化
   for(int i = 0; i < Avenum; i++)   //data[]に格納したAvenum(移動平均の範囲)秒間の値を合計する
     sum_data = sum_data + data[i];
   ave_data = sum_data / Avenum;   //移動平均を計算する
   Serial.print("△安静平均：");
   Serial.println(ave_bpm);   
   Serial.print("○移動平均：");
   Serial.println(ave_data);  
  
   //居眠りの検出処理(LED)
   if(ave_data < ave_bpm*0.9)   //求めた移動平均が安静時の平均BPMを10%以上下回ったら
   {
    digitalWrite(led, HIGH);   //LED点灯
    //Serial.print("1");
   }else 
   {
      digitalWrite(led, LOW);   //下回っていないときは消灯
      //Serial.print("0");
   }

/*
   //画像検出と合わせた最終的な居眠り検出
   int p_data = Serial.read();
   if(int(p_data) == 1)
   {
    printf("【居眠り】");
    digitalWrite(p_led, HIGH);  //最終的な検出結果のLED    
   }else {
    digitalWrite(p_led, LOW);
    printf("起きてる");
   }
   */
  }
  cnt = cnt + 1;   //繰り返し回数(秒数)
 }
}
