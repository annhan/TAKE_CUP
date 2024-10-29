#include "controlCup.h"

CupControl::CupControl(CupPins& pins) : SS_XOAY_CUP_I(pins.xoayCupPin), state(Init) {
    // Sao chép các chân đầu vào và đầu ra vào các biến thành viên
    inputPins = &pins.inputPins;
    outputPins = &pins.outputPins;
}

void CupControl::setup() {
    setupInputPins();
    setupOutputPins();
    attachInterrupt(digitalPinToInterrupt(SS_XOAY_CUP_I), [this]() {this->CupInterruptHandler();}, CHANGE);
}
bool CupControl::isNoCup() {
  if (state == NOCUP){
    return true;
  }
  return false;
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
        case NOCUP:
            handleNOCUP();
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
    pinMode(inputPins->Conveyor_Out, INPUT);
    pinMode(inputPins->Conveyor_IN, INPUT);
    pinMode(inputPins->GRIP_OPEN, INPUT);
    pinMode(inputPins->GRIP_CLOSE, INPUT);
    pinMode(inputPins->MOTOR_UP, INPUT);
    pinMode(inputPins->MOTOR_DW, INPUT);
    pinMode(inputPins->HET_CUP, INPUT);
    pinMode(inputPins->CUP_READY, INPUT);
}

void CupControl::setupOutputPins() {
    pinMode(outputPins->Conveyor_Out, OUTPUT);
    pinMode(outputPins->Conveyor_IN, OUTPUT);
    pinMode(outputPins->GRIP_OPEN, OUTPUT);
    pinMode(outputPins->GRIP_CLOSE, OUTPUT);
    pinMode(outputPins->MOTOR_UP, OUTPUT);
    pinMode(outputPins->MOTOR_DW, OUTPUT);
    pinMode(outputPins->CHANGE_CUP, OUTPUT);
    pinMode(outputPins->XOAY_CUP, OUTPUT);
}

// Các hàm xử lý trạng thái
void CupControl::handleInit() {
    // Logic cho trạng thái Init
    state = DuaVao;
}

void CupControl::handleDuaVao() {
    static int isCHANGE_CUPActive = 0;
    static unsigned long CHANGE_CUPStartTime = 0;
    static bool isWriteDone = false; // Biến static để giữ trạng thái
    int block_ss = digitalRead(inputPins->HET_CUP);
    if (block_ss == LOW){ // có ly thi cu cap nhat trang thai
      if (isCHANGE_CUPActive == 0){
        digitalWrite(outputPins->CHANGE_CUP, TurnOn);  
        isCHANGE_CUPActive = 1;
        CHANGE_CUPStartTime = millis();
      }
      else if (isCHANGE_CUPActive == 3){
          time_begin = 0;
          isCHANGE_CUPActive = 0;
          state = NOCUP;  
      }
      if (isCHANGE_CUPActive == 1 && (millis() - CHANGE_CUPStartTime >= 300)) {
            digitalWrite(outputPins->CHANGE_CUP, TurnOFF);  
            isCHANGE_CUPActive = 2;
      }
      if (isCHANGE_CUPActive == 2 && (millis() - CHANGE_CUPStartTime >= 600)) {
            digitalWrite(outputPins->CHANGE_CUP, TurnOFF);  
            isCHANGE_CUPActive = 3;
      }
    }  

    else {
        if(isWriteDone == false){
              digitalWrite(outputPins->Conveyor_Out, TurnOFF);  // turn the LED on (HIGH is the voltage level)
              digitalWrite(outputPins->Conveyor_IN, TurnOn);  // turn the LED on (HIGH is the voltage level)
              digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);  // turn the LED on (HIGH is the voltage level)
              digitalWrite(outputPins->GRIP_OPEN, TurnOn);  // turn the LED on (HIGH is the voltage level) 
              isWriteDone = true;
        }
        int status_grip = digitalRead(inputPins->GRIP_OPEN);
        if (status_grip == LOW){
          digitalWrite(outputPins->GRIP_OPEN, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
          state = NangCup;
          isWriteDone = false;
          time_begin = 0;
          isCHANGE_CUPActive = 0;
        }
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
    if (interrup_status == 2){
      interrup_status = 0;
      time_begin = 0;
      state = CheckSS;
    }
}

void CupControl::handleCheckSS() {
    // Logic cho trạng thái CheckSS
    int status_grip = digitalRead(inputPins->CUP_GRIP);
    if (status_grip == LOW){ // co cup thi qua state hạ
      state = HaCUp;
    }
    else{
      state = KepCup;
    }
}

void CupControl::handleKepCup() {
    if (time_begin == 0){
      digitalWrite(outputPins->GRIP_OPEN, TurnOFF);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(outputPins->GRIP_CLOSE, TurnOn);  // turn the LED on (HIGH is the voltage level)
      time_begin = millis();
    }
    int status_grip = digitalRead(inputPins->GRIP_CLOSE);
    if (status_grip == LOW){
      time_begin = 0;
      digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);
      state = HaCUp;
    }
    if ((unsigned long)(millis() - time_begin) > 3 * 1000){
      time_begin = 0;
      digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);
      state = HaCUp;
    }
}

void CupControl::handleHaCup50() {
    // Logic cho trạng thái HaCup50
}

void CupControl::handleMOCUP50() {
    // Logic cho trạng thái MOCUP50
}

void CupControl::handleHaCUp() {
    // Logic cho trạng thái HaCUp
    int status_grip = digitalRead(inputPins->Conveyor_IN);
    if (status_grip == LOW){ // co cup thi qua state hạ
          digitalWrite(outputPins->MOTOR_DW, TurnOn);  // turn the LED on (HIGH is the voltage level)
        digitalWrite(outputPins->MOTOR_UP, TurnOFF);  // turn the LED on (HIGH is the voltage level)
        int status_grip = digitalRead(inputPins->MOTOR_DW);
        if (status_grip == LOW){
          digitalWrite(outputPins->MOTOR_DW, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
          state = MoCUP;
        }
    }
}

void CupControl::handleMoCUP() {
    // Logic cho trạng thái MoCUP
    if (time_begin == 0){
      digitalWrite(outputPins->GRIP_CLOSE, TurnOFF);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(outputPins->GRIP_OPEN, TurnOn);  // turn the LED on (HIGH is the voltage level) 
      time_begin = 1;
    }
    else if (time_begin == 1){
      int status_grip = digitalRead(inputPins->GRIP_OPEN);
      if (status_grip == LOW){
        digitalWrite(outputPins->GRIP_OPEN, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
        time_begin = 2;
        state = DuaRa;
      }
    }
    else if (time_begin == 2){
    digitalWrite(outputPins->MOTOR_DW, TurnOFF);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(outputPins->MOTOR_UP, TurnOn);  // turn the LED on (HIGH is the voltage level)
    }
    else if ((unsigned long)(millis() - time_begin) > 3 * 1000){
      time_begin = 0;
      digitalWrite(outputPins->MOTOR_UP, TurnOFF);
      state = DuaRa;
    } 

}

void CupControl::handleDuaRa() {
    digitalWrite(outputPins->Conveyor_IN, TurnOFF);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(outputPins->Conveyor_Out, TurnOn);  // turn the LED on (HIGH is the voltage level)
    int status_grip = digitalRead(inputPins->Conveyor_Out);
    if (status_grip == LOW){
      digitalWrite(outputPins->Conveyor_Out, TurnOFF);  // turn the LED on (HIGH is the voltage level) 
      state = Wait;
    }
}

void CupControl::handleWait() {
  //Chờ robot lấy ly. Nếu không có ly trong 5 giây thì quay về trạng thái đầu để đi lấy ly
  int reading = digitalRead(inputPins->CUP_READY);
  if (reading == HIGH){ // có ly thi cu cap nhat trang thai
    time_begin = millis();
  }
  if ((millis() - time_begin) >  5000) { //5s
        time_begin = 0;
        state = DuaVao;  
  }




}
void CupControl::handleNOCUP() {
  //Chờ có cup lai
}
// Hàm xử lý ngắt
void CupControl::CupInterruptHandler() {
    digitalWrite(outputPins->XOAY_CUP, TurnOFF);  // turn the LED on (HIGH is the voltage level)
    interrup_status = 2;
}

