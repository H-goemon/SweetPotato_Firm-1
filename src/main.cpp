#include "mbed.h"

BufferedSerial pc{USBTX, USBRX, 115200};

CAN can{PA_11, PA_12, (int)1e6};
CANMessage msg;

//ピンの初期設定ゾーン
//速度のPWM出力するピンを初期化する
//命名規則：x_pwm
PwmOut a_pwm(PA_1);
PwmOut b_pwm(PA_3);
PwmOut c_pwm(PA_4);
PwmOut d_pwm(PA_6);

//回転方向の出力をするピンを初期化する
//命名規則：x_dir
DigitalOut a_dir(PA_8);
DigitalOut b_dir(PA_7);
DigitalOut c_dir(PA_10);
DigitalOut d_dir(PA_9);

//デジタル出力するピンを初期化する
//can_read→CAN.read()の真偽値を反映するインジケータ
DigitalOut can_read_LED(PB_6);
//sig_receive→msg.id=(自分のID)の真偽値を反映するインジケータ
DigitalOut sig_receive_LED(PB_7);

//デジタル入力するピンを初期化する
//ロータリスイッチの入力ピン
DigitalIn idset_1(PA_0);
DigitalIn idset_2(PB_0);
DigitalIn idset_4(PB_1);
//ユーザSWの入力ピン
DigitalIn sw(PA_5);

//変数・配列の初期設定
//メインマイコンから送られてきた送信前のデータを復元し、代入する配列
int16_t in_data[4] = {0, 0, 0, 0};
//処理途中のデータを保持する配列
float pwm_data[4] = {0, 0, 0, 0};
//PWM出力のduty比を代入する配列
bool dir_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main(){
  //ロータリスイッチを読み取り、CANIDとして保持(起動時のデータを記録し、変更しない)
  //const int CANID = 1*!idset_1 + 2*!idset_2 + 4*!idset_4;
  //printf("CANID = %d\n",CANID);
  const int CANID = 2;

  //タイムアウト時間
  //constexpr auto timeout = 50ms;

  while(1){
    //タイムアウトの設定
    auto now = HighResClock::now();
    static auto last = now;

    // CANの読み取り
    if(can.read(msg) && msg.id == CANID){
      for(int i = 0; i<4; i++){
        in_data[i] = msg.data[2*i+1] << 8| msg.data[2*i];
      }
      last = now;
    } else {
      for(int i=0; i<4; i++){
        in_data[i] = 0;
      }
    }

    for(int i=0; i<4; i++){
      printf("in_data%d=%d\n",i, in_data[i]);
    }

    in_data[0] = -1000;
    in_data[1] = 2000;
    in_data[2] = -4000;
    in_data[3] = 8000;

    //変数の処理
    //int_16tからfloat型に変換(正負の浮動小数点)
    for(int i=0; i<4; i++){
      pwm_data[i] = float(in_data[i] / 32767.0);
    }

    //出力するデータに変換
    for(int i=0; i<4; i++){
      if(pwm_data[i] > 0){
        dir_data[i] = 0;
      }else if(pwm_data[i] < 0){
        pwm_data[i] = -1*pwm_data[i];
        dir_data[i] = 1;
      }else{
        pwm_data[i] = 0;
        dir_data[i] = 0;
      }
    }

    //PWM出力をする
    a_pwm = pwm_data[0];
    b_pwm = pwm_data[1];
    c_pwm = pwm_data[2];
    d_pwm = pwm_data[3];
    a_dir = dir_data[0];
    b_dir = dir_data[1];
    c_dir = dir_data[2];
    d_dir = dir_data[3];

    // if(can.read(msg) && msg.id==CANID){
    //   sig_receive_LED = 1;
    //   can_read_LED = 0;
    // } else if(can.read(msg) && msg.id!=CANID){
    //   sig_receive_LED = 0;
    //   can_read_LED = 1;
    // } else {
    //   sig_receive_LED = 0;
    //   can_read_LED = 0;
    // }

  }
}