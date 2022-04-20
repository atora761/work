#pragma once
#include <cstdint>
#include <string.h>
#include <memory>
#include <string>

#define SYS_OK 0    //正常終了
#define SYS_NG 1    //異常終了
#define SYS_PARAM 2 //引数エラ
#define SW_PORT4 4
#define SW_PORT5 5
#define SW_PORT6 6
#define SW_PORT13 13
#define SW_PORT18 18
#define SW_PORT19 19

// integer 8bit
using int8 = int8_t;

// integer 16bit
using int16 = int16_t;

// integer 32bit
using int32 = int32_t;

// integer 64bit
using int64 = int64_t;

// unsigned integer 8bit
using uint8 = uint8_t;

// unsigned integer 16bit
using uint16 = uint16_t;

// unsigned integer 32bit
using uint32 = uint32_t;

// unsigned integer 64bit
using uint64 = uint64_t;

// int pointer
using intptr = intptr_t;

// uint pointer
using uintptr = uintptr_t;
enum WorkerState
{
    //準備状態
    PREPARATION_MODE,
    //デバイス系統の初期化状態
    DEVICEINIT_MODE,
    //前アームの限界値への移動状態
    FRONTARM_LIMITSERCH,
    //左下アーム初期位置への移動状態
    FRONTARMINIT_MODE ,
    //後ろアームの限界値への移動状態
    BACKARM_LIMITSERCH,
    //左上アーム初期位置への移動状態
    BACKARMINIT_MODE ,
    //正常運転状態
    NOMAL_MODE,
    //終了状態
    END_MODE,
};
enum TriggerState
{
    //緊急ボタン
    EMERGENCY_SWITCH,
    //アーム上の緊急停止スイッチ*4
    EMERGENCY_LEFTUP,
    EMERGENCY_LEFTDOWN,
    EMERGENCY_RIGHTUP,
    EMERGENCY_RIGHTDOWN,
};
typedef struct CONTOUR_TAG {	
	double x;
    double y;
}CONTOUR_DATE;


