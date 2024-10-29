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
        uint8_t BangTai_RA;
        uint8_t BangTai_VAO;
        uint8_t GRIP_OPEN;
        uint8_t GRIP_CLOSE;
        uint8_t MOTOR_UP;
        uint8_t MOTOR_DW;
        uint8_t HETCUP;
        uint8_t CUP_READY;
        uint8_t CUP_GRIP;
    } inputPins;

    struct CTR_OUT {
        uint8_t BangTai_RA;
        uint8_t BangTai_VAO;
        uint8_t GRIP_OPEN;
        uint8_t GRIP_CLOSE;
        uint8_t MOTOR_UP;
        uint8_t MOTOR_DW;
        uint8_t VANCUP;
        uint8_t XOAY_CUP;
    } outputPins;
};

// Enum cho trạng thái của Cup
class CupControl {
  public:
      enum State {
          Init,
          DuaVao,
          NangCup,
          KepCup50,
          XoayCUp,
          CheckSS,
          KepCup,
          HaCup50,
          MOCUP50,
          HaCUp,
          MoCUP,
          DuaRa,
          Wait
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

  private:
      volatile interrup_status = false;
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

      // Hàm xử lý ngắt
      static void CupInterruptHandler();
};

#endif // CUP_H
