#include "mbed.h"
#include "array"

BufferedSerial pc{USBTX, USBRX, 115200};

CAN can{PA_11, PA_12, (int)1e6};
CANMessage msg;

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
DigitalOut can_read(PB_6);
DigitalOut sig_receive(PB_7);

//デジタル入力するピンを初期化する
//ロータリースイッチの入力ピン
DigitalIn idset_1(PA_0);
DigitalIn idset_2(PB_0);
DigitalIn idset_4(PB_1);
//ユーザSWの入力ピン
DigitalIn sw(PA_5);

int main(){
  while(1){

  }
}