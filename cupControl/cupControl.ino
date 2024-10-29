#include <ModbusRtu.h>
#include "controlCup.h"

#define TXEN_PIN PA11

Modbus slave(1, Serial2, TXEN_PIN); // this is slave @1 and RS-485

CupPins cup2Pins = {
    .xoayCupPin = PA1,
    .inputPins = {
        .BangTai_RA = PA7,
        .BangTai_VAO = PC_4,
        .GRIP_OPEN = PC_5,
        .GRIP_CLOSE = PB0,
        .MOTOR_UP = PB1,
        .MOTOR_DW = PB10,
        .HETCUP = PB11,
        .CUP_READY = PB12,
        .CUP_GRIP = PB13
    },
    .outputPins = {
        .BangTai_RA = PC_10,
        .BangTai_VAO = PC_11,
        .GRIP_OPEN = PC_12,
        .GRIP_CLOSE = PB3,
        .MOTOR_UP = PB5,
        .MOTOR_DW = PB4,
        
        .VANCUP = PB6,
        .XOAY_CUP = PB7
    }
};

CupPins cup1Pins = {
    .xoayCupPin = PA0,
    .inputPins = {
        .BangTai_RA = PC13,
        .BangTai_VAO = PC14,
        .GRIP_OPEN = PC_0,
        .GRIP_CLOSE = PC_15,
        .MOTOR_UP = PC_1,
        .MOTOR_DW = PC_2,
        .HETCUP = PC_3,
        .CUP_READY = PA5,
        .CUP_GRIP = PA6
    },
    .outputPins = {
        .BangTai_RA = PB14,
        .BangTai_VAO = PB15,
        .GRIP_OPEN = PC_6,
        .GRIP_CLOSE = PC_7,
        .MOTOR_UP = PC_9,
        .MOTOR_DW = PC_8,
        .VANCUP = PA8,
        .XOAY_CUP = PA15
    }
};

CupControl CupControl1(cup1Pins); // Khởi tạo đối tượng Cup với các chân tương ứng
CupControl CupControl2(cup2Pins); // Khởi tạo đối tượng Cup với các chân tương ứng

void setup() {
    CupControl1.setup(); // Gọi hàm setup để thiết lập các chân
    CupControl2.setup(); // Gọi hàm setup để thiết lập các chân
}

uint16_t au16data[7] = {0, 0, 0, 0, 0, 0, 0 }; 

void loop() {

    CupControl1.control(); // Gọi hàm control để điều khiển Cup
    CupControl2.control(); // Gọi hàm control để điều khiển Cup
    au16data[0] = CupControl1.getState();
    au16data[1] = CupControl1.getState();
    slave.poll( au16data, 7);
    
}