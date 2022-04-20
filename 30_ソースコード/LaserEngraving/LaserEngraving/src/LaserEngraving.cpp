#include "../include/LaserEngraving.hpp"
//割込み処理のチャタリング用タイマー
unsigned int uintTime=0;
unsigned int uintPreTime=0;
/* -------------------------------------------------------------------------- */
/* 関数名       ：EmergencyInterrupt                                           */
/* 機能名       ：緊急停止割込み                                                */
/* 機能概要     ：緊急停止スイッチが押された場合に動作する処理                     */
/*               状態管理に緊急停止が押されたというフラグを立てる                  */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
void EmergencyInterrupt(){
    //現在のプログラム開始時間を取得
    uintTime=millis();

    //チャタリング処理
    //前回の取得時間と比較して500ms以下の感覚だった場合処理をせずに終了
    if(500>(uintTime-uintPreTime)){
        //前回の割込み時間を更新
        uintPreTime=uintTime;
        return;
    }

    //前回の割込み時間を更新
    uintPreTime=uintTime;

    //状態管理クラスにボタンが押されたという状態をセット
    StateManage &statemanage=StateManage::getInstance();
    statemanage.triggerStateSetter(EMERGENCY_SWITCH);
    return;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：LeftUpInterrupt                                              */
/* 機能名       ：左上アームスイッチ割込み                                       */
/* 機能概要     ：左上アームスイッチが押された場合に動作する処理                   */
/*               状態管理に左上アームスイッチが押されたというフラグを立てる        */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
void LeftUpInterrupt(){
    //現在のプログラム開始時間を取得
    uintTime=millis();    

    //チャタリング処理
    //前回の取得時間と比較して500ms以下の感覚だった場合処理をせずに終了
    if(500>(uintTime-uintPreTime)){
        //前回の割込み時間を更新
	    uintPreTime=uintTime;
	    return;
    }
    //前回の割込み時間を更新
    uintPreTime=uintTime;
  
    //状態管理クラスにボタンが押されたという状態をセット  
    StateManage &statemanage=StateManage::getInstance();
    statemanage.triggerStateSetter(EMERGENCY_LEFTUP);
    return;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：LeftDownInterrupt                                            */
/* 機能名       ：左下アームスイッチ割込み                                       */
/* 機能概要     ：左下アームスイッチが押された場合に動作する処理                   */
/*               状態管理に左下アームスイッチが押されたというフラグを立てる        */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
void LeftDownInterrupt(){
    //現在のプログラム開始時間を取得
    uintTime=millis();    

    //チャタリング処理
    //前回の取得時間と比較して500ms以下の感覚だった場合処理をせずに終了
    if(500>(uintTime-uintPreTime)){
        //前回の割込み時間を更新
        uintPreTime=uintTime;
        return;
    }

    //前回の割込み時間を更新
    uintPreTime=uintTime;

    //状態管理クラスにボタンが押されたという状態をセット  
    StateManage &statemanage=StateManage::getInstance();
    statemanage.triggerStateSetter(EMERGENCY_LEFTDOWN);
    return;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：RightUpInterrupt                                             */
/* 機能名       ：右上アームスイッチ割込み                                       */
/* 機能概要     ：右上アームスイッチが押された場合に動作する処理                   */
/*               状態管理に右上アームスイッチが押されたというフラグを立てる        */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
void RightUpInterrupt(){
    //現在のプログラム開始時間を取得
    uintTime=millis();    

    //チャタリング処理
    //前回の取得時間と比較して500ms以下の感覚だった場合処理をせずに終了
    if(500>(uintTime-uintPreTime)){
        //前回の割込み時間を更新
	    uintPreTime=uintTime;
	    return;
    }
    
    //前回の割込み時間を更新
    uintPreTime=uintTime;

    //状態管理クラスにボタンが押されたという状態をセット  
    StateManage &statemanage=StateManage::getInstance();
    statemanage.triggerStateSetter(EMERGENCY_RIGHTUP);
    return;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：RightDownInterrupt                                           */
/* 機能名       ：右下アームスイッチ割込み                                       */
/* 機能概要     ：右下アームスイッチが押された場合に動作する処理                   */
/*               状態管理に左上アームスイッチが押されたというフラグを立てる        */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
void RightDownInterrupt(){
    //現在のプログラム開始時間を取得
    uintTime=millis(); 

    //チャタリング処理
    //前回の取得時間と比較して500ms以下の感覚だった場合処理をせずに終了   
    if(500>(uintTime-uintPreTime)){
        //前回の割込み時間を更新
        uintPreTime=uintTime;
        return;
    }

    //前回の割込み時間を更新
    uintPreTime=uintTime;

    //状態管理クラスにボタンが押されたという状態をセット  
    StateManage &statemanage=StateManage::getInstance();
    statemanage.triggerStateSetter(EMERGENCY_RIGHTDOWN);
    return;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：コンストラクタ                                                */
/* 機能名       ：初期化処理                                                　  */
/* 機能概要     ：スイッチ信号取得とレーザーのpwm制御をするために行う初期化処理     */
/* 引数         ：なし                                                         */
/* 戻り値       ：なし                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
LaserEngraving::LaserEngraving(){
    //現在のプログラム開始時間を取得
	uintTime=millis(); 

    //ラズベリーパイのioピンの初期化処理
	if(wiringPiSetupGpio() < 0) return ;
	pinMode(SW_PORT4,INPUT);
	pinMode(SW_PORT5,INPUT);
	pinMode(SW_PORT6,INPUT);
	pinMode(SW_PORT13,INPUT);
	pinMode(SW_PORT18, PWM_OUTPUT);
	pinMode(SW_PORT19,INPUT);

	pullUpDnControl(SW_PORT4,PUD_DOWN);
	pullUpDnControl(SW_PORT5,PUD_DOWN);
	pullUpDnControl(SW_PORT6,PUD_DOWN);
	pullUpDnControl(SW_PORT13,PUD_DOWN);
	pullUpDnControl(SW_PORT19,PUD_DOWN);

	wiringPiISR(SW_PORT4,INT_EDGE_RISING,EmergencyInterrupt);
	wiringPiISR(SW_PORT5,INT_EDGE_RISING,LeftUpInterrupt);
	wiringPiISR(SW_PORT6,INT_EDGE_RISING,LeftDownInterrupt);
	wiringPiISR(SW_PORT13,INT_EDGE_RISING,RightUpInterrupt);
	wiringPiISR(SW_PORT19,INT_EDGE_RISING,RightDownInterrupt);
}

LaserEngraving::~LaserEngraving(){}

/* -------------------------------------------------------------------------- */
/* 関数名       ：run                                                          */
/* 機能名       ：レーザー彫刻処理の実行                                     　  */
/* 機能概要     ：引数のファイルパスを画像取得に受け渡し、輪郭データを　受け取る     */
/*               その後、彫刻制御に輪郭データを受け渡す                           */
/* 引数         ：char* ,画像のパス                                             */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int LaserEngraving::run(char* _filepath){

    //輪郭取得クラス宣言
    InputImage inputimage;

    //彫刻制御クラス宣言
    EngravingControl engravingcontrol;

    //輪郭情報格納用
    std::map<int,std::list<CONTOUR_DATE>> contour;

    //初期化
    init();

    //画像データ取得
    inputimage.input(_filepath);

    //生成された輪郭データを取得
    inputimage.contourGetter(&contour);

    //彫刻制御のrun
    engravingcontrol.run(contour);

    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：init                                                         */
/* 機能名       ：レーザー彫刻処理の初期化処理                                　  */
/* 機能概要     ：前回動作しぱなっしにしたときのための安全策                       */
/* 引数         ：void                                                         */
/* 戻り値       ：void                                                         */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int LaserEngraving::init(){
    //彫刻制御クラス宣言
    EngravingControl engravingcontrol;
    
    //彫刻制御を停止
    engravingcontrol.stop();
    
    return SYS_OK;
}

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */