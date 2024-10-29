#include "controlCup.h"

CupControl::CupControl(CupPins& pins) : SS_XOAY_CUP_I(pins.xoayCupPin), state(Init) {
          // Sao chép các chân đầu vào và đầu ra vào các biến thành viên
          inputPins = &pins.inputPins;
          outputPins = &pins.outputPins;
}

void CupControl::setup() {
    setupInputPins();
    setupOutputPins();
    attachInterrupt(digitalPinToInterrupt(SS_XOAY_CUP_I), &CupControl::CupInterruptHandler, CHANGE);
}

void CupControl::control() {
    switch (state) {
        case Init:
            handleInit();
            break;
        case DuaVao:
            handleDuaVao();
            break;
        case NangCup:
            handleNangCup();
            break;
        case KepCup50:
            handleKepCup50();
            break;
        case XoayCUp:
            handleXoayCUp();
            break;
        case CheckSS:
            handleCheckSS();
            break;
        case KepCup:
            handleKepCup();
            break;
        case HaCup50:
            handleHaCup50();
            break;
        case MOCUP50:
            handleMOCUP50();
            break;
        case HaCUp:
            handleHaCUp();
            break;
        case MoCUP:
            handleMoCUP();
            break;
        case DuaRa:
            handleDuaRa();
            break;
        case Wait:
            handleWait();
            break;
    }
}

void CupControl::setState(State newState) {
    state = newState;
}

CupControl::State CupControl::getState() {
    return state;
}



void CupControl::setupInputPins() {
    pinMode(inputPins->BangTai_RA, INPUT);
    pinMode(inputPins->BangTai_VAO, INPUT);
    pinMode(inputPins->GRIP_OPEN, INPUT);
    pinMode(inputPins->GRIP_CLOSE, INPUT);
    pinMode(inputPins->MOTOR_UP, INPUT);
    pinMode(inputPins->MOTOR_DW, INPUT);
    pinMode(inputPins->HETCUP, INPUT);
    pinMode(inputPins->CUP_READY, INPUT);
}

void CupControl::setupOutputPins() {
    pinMode(outputPins->BangTai_RA, OUTPUT);
    pinMode(outputPins->BangTai_VAO, OUTPUT);
    pinMode(outputPins->GRIP_OPEN, OUTPUT);
    pinMode(outputPins->GRIP_CLOSE, OUTPUT);
    pinMode(outputPins->MOTOR_UP, OUTPUT);
    pinMode(outputPins->MOTOR_DW, OUTPUT);
    pinMode(outputPins->VANCUP, OUTPUT);
    pinMode(outputPins->XOAY_CUP, OUTPUT);
}

// Các hàm xử lý trạng thái
void CupControl::handleInit() {
    // Logic cho trạng thái Init
    state = DuaVao;
}

void CupControl::handleDuaVao() {
    // Logic cho trạng thái DuaVao
    // đưa băng tải vào, mở càng gắp và chờ mở grip
    static bool isWriteDone = false; // Biến static để giữ trạng thái
    if(isWriteDone == false){
          digitalWrite(outputPins->BangTai_RA, TurnOFF);  // turn the LED on (HIGH is the voltage level)
          digitalWrite(outputPins->BangTai_VAO, TurnOn);  // turn the LED on (HIGH is the voltage level)
          digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);  // turn the LED on (HIGH is the voltage level)
          digitalWrite(outputPins->GRIP_OPEN, TurnOn);  // turn the LED on (HIGH is the voltage level) 
          isWriteDone = true;
    }
    int status_grip = digitalRead(inputPins->GRIP_OPEN);
    if (status_grip == LOW){
      digitalWrite(outputPins->GRIP_OPEN, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
      state = NangCup;
      isWriteDone = false;
    }
}

void CupControl::handleNangCup() {
    // Logic cho trạng thái NangCup
    digitalWrite(outputPins->MOTOR_DW, TurnOFF);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(outputPins->MOTOR_UP, TurnOn);  // turn the LED on (HIGH is the voltage level)
    int status_grip = digitalRead(inputPins->MOTOR_UP);
    if (status_grip == LOW){
      digitalWrite(outputPins->MOTOR_UP, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
      state = KepCup50;
    }
}

void CupControl::handleKepCup50() {
    // Logic cho trạng thái KepCup50
    if (time_begin == 0){
      digitalWrite(outputPins->GRIP_OPEN, TurnOFF);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(outputPins->GRIP_CLOSE, TurnOn);  // turn the LED on (HIGH is the voltage level)
      time_begin = millis();
    }
    if ((unsigned long)(millis() - time_begin) > 3 * 1000){
      time_begin = 0;
      digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);
      state = XoayCUp;
    }
}

void CupControl::handleXoayCUp() {
    if (time_begin == 0){
      interrup_status = false;
      digitalWrite(outputPins->XOAY_CUP, TurnOn);  // turn the LED on (HIGH is the voltage level)
      time_begin = millis();
    }
    if (interrup_status == true){
      interrup_status = false;
      time_begin = 0;
      state = CheckSS;
    }
}

void CupControl::handleCheckSS() {
    // Logic cho trạng thái CheckSS
}

void CupControl::handleKepCup() {
    // Logic cho trạng thái KepCup
}

void CupControl::handleHaCup50() {
    // Logic cho trạng thái HaCup50
}

void CupControl::handleMOCUP50() {
    // Logic cho trạng thái MOCUP50
}

void CupControl::handleHaCUp() {
    // Logic cho trạng thái HaCUp
}

void CupControl::handleMoCUP() {
    // Logic cho trạng thái MoCUP
}

void CupControl::handleDuaRa() {
    // Logic cho trạng thái DuaRa
}

void CupControl::handleWait() {
    // Logic cho trạng thái Wait
}

// Hàm xử lý ngắt
void CupControl::CupInterruptHandler() {
    digitalWrite(outputPins->XOAY_CUP, TurnOff);  // turn the LED on (HIGH is the voltage level)
    interrup_status = 2;
}

