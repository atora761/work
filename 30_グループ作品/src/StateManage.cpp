#include "../include/StateManage.hpp"
//コンストラクタ
StateManage::StateManage(){}
//デストラクタ
StateManage::~StateManage(){}
/* -------------------------------------------------------------------------- */
/* 関数名       ：stateGetter                                                  */
/* 機能名       ：現在状態の取得                                            　  */
/* 機能概要     ：引数に現在の状態を格納する                                     */
/* 引数         ：int* ,int型のポインタ変数                                     */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int StateManage::stateGetter(int* _inputstate){
    //引数チェック
    if(_inputstate==NULL){
        return SYS_PARAM;
    }
    *_inputstate=curent_state;
    return SYS_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：stateSetter                                                  */
/* 機能名       ：現在状態の更新                                            　  */
/* 機能概要     ：引数の現在状態を格納する                                       */
/* 引数         ：int ,int型の変数                                             */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int StateManage::stateSetter(int _inputstate){
    curent_state=_inputstate;
    return SYS_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名       ：triggerStateSetter                                           */
/* 機能名       ：ボタンイベントにおける状態更新                              　  */
/* 機能概要     ：状態テーブルと照らし合わせて、引数に現在の状態を格納する          */
/* 引数         ：int ,int型の変数                                             */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int StateManage::triggerStateSetter(int _inputstate){
    curent_state=state_table[_inputstate][curent_state];
    return SYS_OK;
}

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */