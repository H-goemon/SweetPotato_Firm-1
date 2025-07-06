#include "mbed.h"
#include "array"

BufferedSerial pc{USBTX, USBRX, 115200};

CAN can{PA_11, PA_12, (int)1e6};
CANMessage msg;

//ピンの初期設定ゾーン
//PWM出力するピンを初期化する
//命名規則：x_y (x:ポート,y:fwd→前転,rev→後転)
PwmOut a_fwd(PA_1);
PwmOut a_rev(PA_3);
PwmOut b_fwd(PA_4);
PwmOut b_rev(PA_6);
PwmOut c_fwd(PA_8);
PwmOut c_rev(PA_7);
PwmOut d_fwd(PA_10);
PwmOut d_rev(PA_9);

//デジタル出力するピンを初期化する
//can_read→CAN.read()の真偽値を反映するインジケータ
DigitalOut can_read(PB_6);
//sig_receive→msg.id=(自分のID)の真偽値を反映するインジケータ
DigitalOut sig_receive(PB_7);

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
//PWM出力のduty比を代入する配列
float out_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main(){
  //ロータリスイッチを読み取り、CANIDとして保持
  const int CANID = 1*!idset_1 + 2*!idset_2 + 4*!idset_4;

  //タイムアウト時間
  constexpr auto timeout = 50ms;

  while(1){
    //タイムアウトの設定
    auto no = HighResClock::();
    static auto last = now;
    
    //CANの読み取り
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

    //PWM出力をする
    a_fwd = out_data[0];
    a_rev = out_data[1];
    b_fwd = out_data[2];
    b_rev = out_data[3];
    c_fwd = out_data[4];
    c_rev = out_data[5];
    d_fwd = out_data[6];
    d_rev = out_data[7];
  }
}