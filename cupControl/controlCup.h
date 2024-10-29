#ifndef CONTROCUP_H
#define CONTROCUP_H

#include <Arduino.h> // Bao gồm thư viện Arduino để sử dụng các hàm như pinMode
#define TurnOn 1
#define TurnOFF 0
// Định nghĩa cấu trúc chân cho Cup
struct CupPins {
    uint8_t xoayCupPin; // Chân tín hiệu cho cốc xoay interrup
    // Các chân khác nếu cần
    struct SS_IN {
        uint8_t Conveyor_Out;
        uint8_t Conveyor_IN;
        uint8_t GRIP_OPEN;
        uint8_t GRIP_CLOSE;
        uint8_t MOTOR_UP;
        uint8_t MOTOR_DW;
        uint8_t HET_CUP;
        uint8_t CUP_READY;
        uint8_t CUP_GRIP;
    } inputPins;

    struct CTR_OUT {
        uint8_t Conveyor_Out;
        uint8_t Conveyor_IN;
        uint8_t GRIP_OPEN;
        uint8_t GRIP_CLOSE;
        uint8_t MOTOR_UP;
        uint8_t MOTOR_DW;
        uint8_t CHANGE_CUP;
        uint8_t XOAY_CUP;
    } outputPins;
};

// Enum cho trạng thái của Cup
class CupControl {
  public:
      enum State {
          Init=0,
          DuaVao=1,
          NangCup=2,
          KepCup50=3,
          XoayCUp=4,
          CheckSS=5,
          KepCup=6,
          HaCup50=7,
          MOCUP50=8,
          HaCUp=9,
          MoCUP=10,
          DuaRa=11,
          Wait=12,
          NOCUP=20
      };

      // Constructor
      CupControl(CupPins& pins);

      // Thiết lập các chân
      void setup();

      // Điều khiển Cup
      void control();

      // Thiết lập trạng thái
      void setState(State newState);
      State getState();
      bool isNoCup();
// Hàm xử lý ngắt
      void CupInterruptHandler();
  private:
      volatile int interrup_status = false;
      uint8_t SS_XOAY_CUP_I; // Chân tín hiệu cho cốc
      unsigned long time_begin = 0;
      CupPins::SS_IN *inputPins;     // Các chân đầu vào
      CupPins::CTR_OUT *outputPins;    // Các chân đầu ra
      State state;           // Trạng thái hiện tại của cốc

      // Các hàm thiết lập chân
      void setupInputPins();
      void setupOutputPins();

      // Các hàm xử lý trạng thái
      void handleInit();
      void handleDuaVao();
      void handleNangCup();
      void handleKepCup50();
      void handleXoayCUp();
      void handleCheckSS();
      void handleKepCup();
      void handleHaCup50();
      void handleMOCUP50();
      void handleHaCUp();
      void handleMoCUP();
      void handleDuaRa();
      void handleWait();
      void handleNOCUP();
      
};

#endif // CUP_H
