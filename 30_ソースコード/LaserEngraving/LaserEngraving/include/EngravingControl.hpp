#pragma once

/* ------------------------------------------------------------------------- */
/* EngravingControl.hpp														 */
/* 彫刻制御のクラス														      */
/* 																			 */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		   */
/* ------------------------------------------------------------------------- */
/* 000000	新規作成								2022/01/29	渡部 湧也  	  */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* includeファイル															 */
/* ------------------------------------------------------------------------- */
#include "./StateManage.hpp"
#include "../include/ArmControl.hpp"
#include "../include/Laser.hpp"
#include "System.hpp"
//opencvのライブラリ取得
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <wiringPi.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <list>
/* ------------------------------------------------------------------------- */
/* define宣言																 */
/* ------------------------------------------------------------------------- */
#define ARM_INIT_ANGLE_140 19750
#define ARM_INIT_ANGLE_25 2844
#define DUTY_RATIO_RUN 80
#define DUTY_RATIO_STOP 0

/* ------------------------------------------------------------------------- */
/* class宣言																 */
/* ------------------------------------------------------------------------- */
class EngravingControl {
    public:
        //コンストラクタ
        EngravingControl();
        //デストラクタ
        ~EngravingControl();
        //実行
        int run(std::map<int,std::list<CONTOUR_DATE>>);
        //停止
        int stop();
    private:
        //初期化動作
        int initMove();
        //アームの限界値に移動
        int limitSercher(int dir,char select);
        //前方アームを初期位置に移動
        int frontArmInit();
        //後方アームを初期位置に移動
        int backArmInit();
        //通常動作
        int nomalMove();
        //状態管理インスタンス取得
        StateManage &statemanage=StateManage::getInstance();
        int movement_state=0;
        //階層輪郭のmap情報
        std::map<int,std::list<CONTOUR_DATE>> contour_date;
        //輪郭座標のリスト情報
        std::list<CONTOUR_DATE> coordinate_data;
        //階層輪郭のmap情報の索引
        std::map<int,std::list<CONTOUR_DATE>>::iterator contour_index;
        //輪郭座標のリスト情報の索引
        std::list<CONTOUR_DATE>::iterator coordinate_index;
};

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */