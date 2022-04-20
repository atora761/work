#include "../include/EngravingControl.hpp"
//コンストラクタ
EngravingControl::EngravingControl(){}
//デストラクタ
EngravingControl::~EngravingControl(){}
/* -------------------------------------------------------------------------- */
/* 関数名       ：run                                                          */
/* 機能名       ：彫刻処理の実行                                             　 */
/* 機能概要     ：アームのキャリブレーション及び、アームの動作座標の受け渡し、      */
/*               レーザーの制御                                                */
/* 引数         ：std::map<int,std::list<CONTOUR_DATE>>:輪郭データ             */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::run(std::map<int,std::list<CONTOUR_DATE>> _contour_data){
    //引数の輪郭データ取得
    contour_date=_contour_data;
    
    //輪郭データのイテレーターを設定
    //階層分けされた輪郭の先頭イテレータを取得
    contour_index=contour_date.begin();

    //先頭輪郭の座標が格納されたリストを取得
    coordinate_data=contour_index->second;

    //輪郭の先頭座標のイテレータを取得
    coordinate_index=coordinate_data.begin();

    //メインループ(彫刻が終了するか、緊急停止が押された場合ループを抜ける)
    while(1){
        //現在の最新状態を取得
        statemanage.stateGetter(&movement_state);

        switch(movement_state){
            //準備状態
            case PREPARATION_MODE:
                //扉、作業ボタン緊急停止が動通時に入る割込みによる状態遷移待機状態
                break;
            //初期化状態
            case DEVICEINIT_MODE:
                //初期化動作(アームの初期座標調整)
                initMove();
                break;
            //左下アームの限界点への移動
            case FRONTARM_LIMITSERCH:
                limitSercher(LEFT_ROTATE,FRONT);
                break;
            //左下アームの初期地点への移動
            case FRONTARMINIT_MODE:
                frontArmInit();
                break;
            //左上アームの限界点への移動
            case BACKARM_LIMITSERCH:
                limitSercher(LEFT_ROTATE,BACK);
                break;
            //左上アームの初期化
            case BACKARMINIT_MODE:
                backArmInit();
                break;
            //通常動作状態
            case NOMAL_MODE:
                //通常動作(レーザー彫刻を行う)
                nomalMove();
                break;
            //終了動作状態
            case END_MODE:
                //彫刻終了または緊急停止時に入る状態
                //アームとレーザーの停止
                stop();
                return SYS_OK;
                break;
            default:
                break;
        }
    }
    return SYS_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：initMove                                                    */
/* 機能名       ：彫刻処理の初期化                                           　 */
/* 機能概要     : アームのキャリブレーション                                     */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::initMove(){
    //アーム制御インスタンス取得
    ArmControl &armcontrol = ArmControl::getInstance();
    Laser &laser=Laser::getInstance();
    //レーザーの初期化処理
    laser.init();
    stop();
    statemanage.stateSetter(FRONTARM_LIMITSERCH);
    return SYS_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：limitSercher                                                */
/* 機能名       ：アームを限界値移動                                         　 */
/* 機能概要     : アームの限界値に移動するために使用                             */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::limitSercher(int dir,char select){
    //アーム制御インスタンス取得
    ArmControl &armcontrol = ArmControl::getInstance();
    armcontrol.manualMode(dir,select);
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：frontArmInit                                                 */
/* 機能名       ：先方アームの初期化                                         　 */
/* 機能概要     : 先方アームを初期位置に移動させる                               */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::frontArmInit(){
    //アーム制御インスタンス取得
    ArmControl &armcontrol = ArmControl::getInstance();
    //アーム停止
    armcontrol.stop();

    // アームスイッチから正面位置に戻るために行う処理
    for(int index=0;index<ARM_INIT_ANGLE_140;++index){
        armcontrol.manualMode(RIGHT_ROTATE,FRONT);
    }
    statemanage.stateSetter(BACKARM_LIMITSERCH);
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：backArmInit                                                 */
/* 機能名       ：後方アームの初期化                                         　 */
/* 機能概要     : 後方アームを初期位置に移動させる                               */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::backArmInit(){
    //アーム制御インスタンス取得
    ArmControl &armcontrol = ArmControl::getInstance();
    //アーム停止
    armcontrol.stop();

    // アームスイッチからアーム座標の初期位置に戻るために行う処理
    for(int index=0;index<ARM_INIT_ANGLE_25;++index){
         armcontrol.manualMode(RIGHT_ROTATE,BACK);
    }
    statemanage.stateSetter(NOMAL_MODE);
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：nomalMove                                                    */
/* 機能名       ：彫刻処理                                                   　 */
/* 機能概要     : アームへの座標受け渡し、レーザー制御                            */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::nomalMove(){
    Laser &laser=Laser::getInstance();

    //現在の輪郭の頂点リストを取得し終わったか確認
    if(coordinate_index==coordinate_data.end()){

        //次の階層輪郭へ移行
        contour_index++;
        
        //輪郭データの終了確認
        if(contour_index==contour_date.end()){
            //動作終了
            statemanage.stateSetter(END_MODE);
            return SYS_OK;
        }
        
        //輪郭の座標が格納されたリストを取得
        coordinate_data=contour_index->second;
        //輪郭の先頭座標のイテレータを取得
        coordinate_index=coordinate_data.begin();
        
    }
    //輪郭の先頭座標の場合レーザーOFF、それ以外の場合はレーザーON
    if(coordinate_index==coordinate_data.begin()){
        laser.stop();
    }else{
        laser.run(DUTY_RATIO_RUN);
    }

    //アーム制御インスタンス取得
    ArmControl &armcontrol=ArmControl::getInstance();

    //アームに座標引き渡し
    armcontrol.run((double)coordinate_index->x,(double)coordinate_index->y);
    
    //次の座標リストへ
    coordinate_index++;
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：stop                                                         */
/* 機能名       ：制御物の停止                                               　 */
/* 機能概要     : アームとレーザー停止  　                                       */
/* 引数         : void                                                        */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int EngravingControl::stop(){
    ArmControl &armcontrol=ArmControl::getInstance();
    Laser &laser=Laser::getInstance();
    
    //レーザー停止
    laser.stop();

    //アーム停止
    armcontrol.stop();
    return SYS_OK;
}

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */