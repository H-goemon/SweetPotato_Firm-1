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
PwmOut c_fwd(PB_0);
PwmOut c_rev(PB_1);
PwmOut d_fwd(PA_10);
PwmOut d_rev(PA_9);

//デジタル出力するピンを初期化する
//can_read→CAN.read()の真偽値を反映するインジケータ
DigitalOut can_read_LED(PB_6);
//sig_receive→msg.id=(自分のID)の真偽値を反映するインジケータ
DigitalOut sig_receive_LED(PB_7);

// //デジタル入力するピンを初期化する
// //ロータリスイッチの入力ピン
// DigitalIn idset_1(PA_0);
// DigitalIn idset_2(PB_0);
// DigitalIn idset_4(PB_1);
//ユーザSWの入力ピン
DigitalIn sw(PA_5);

//変数・配列の初期設定
//メインマイコンから送られてきた送信前のデータを復元し、代入する配列
int16_t in_data[4] = {0, 0, 0, 0};
//処理途中のデータを保持する配列
float process_data[4] = {0, 0, 0, 0};
//PWM出力のduty比を代入する配列
float out_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main(){
  //ロータリスイッチを読み取り、CANIDとして保持(起動時のデータを記録し、変更しない)
  // const int CANID = 1*!idset_1 + 2*!idset_2 + 4*!idset_4;
  const int CANID = 10;
  printf("CANID = %d\n",CANID);

  //タイムアウト時間
  constexpr auto timeout = 50ms;

  while(1){
    //タイムアウトの設定
    auto now = HighResClock::now();
    static auto last = now;

    // //CANの読み取り
    // if(can.read(msg) && msg.id == CANID){
    //   for(int i = 0; i<4; i++){
    //     in_data[i] = msg.data[2*i+1] << 8| msg.data[2*i];
    //   }
    //   last = now;
    // } else {
    //   for(int i=0; i<4; i++){
    //     in_data[i] = 0;
    //   }
    // }

    in_data[0] = -1000;
    in_data[1] = -2000;
    in_data[2] = -3000;
    in_data[3] = -4000;

    for(int i=0; i<4; i++){
      printf("in_data%d=%d\n",i, in_data[i]);
    }

    //変数の処理
    //int_16tからfloat型に変換(正負の浮動小数点)
    for(int i=0; i<4; i++){
      process_data[i] = float(in_data[i] / 32767.0);
      printf("process_data%d=%f\n", i, process_data[i]);
    }
    //出力するデータに変換
    for(int i=0; i<4; i++){
      if(process_data[i] > 0){
        out_data[2*i] = process_data[i];
        out_data[2*i+1] = 0;
      }else if(process_data[i] < 0){
        out_data[2*i] = 0;
        out_data[2*i+1] = -1*process_data[i];
      }else{
        out_data[2*i] = 0;
        out_data[2*i+1] = 0;
      }
    }
    for(int i=0; i<8; i++){
      printf("out_data%d=%f\n", i, out_data[i]);
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