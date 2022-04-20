#pragma once
/* ------------------------------------------------------------------------- */
/* StateManage.hpp															 */
/* 状態管理のクラス			     											  */
/* 																			 */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		   */
/* ------------------------------------------------------------------------- */
/* 000000	新規作成								2022/01/29	渡部 湧也  	  */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* includeファイル															  */
/* ------------------------------------------------------------------------- */
#include "./System.hpp"

/* ------------------------------------------------------------------------- */
/* define宣言																 */
/* ------------------------------------------------------------------------- */
#define MAIN_EVENT 8
#define TRIGGER_EVENT 5
//状態遷移テーブル
const int state_table[TRIGGER_EVENT][MAIN_EVENT]={
    {DEVICEINIT_MODE,END_MODE,END_MODE         ,END_MODE  ,END_MODE        ,END_MODE,END_MODE,END_MODE},//緊急停止
    {END_MODE ,      END_MODE,FRONTARMINIT_MODE,END_MODE  ,END_MODE        ,END_MODE,END_MODE,END_MODE},//アーム(左上)
    {END_MODE ,      END_MODE,END_MODE         ,END_MODE  ,BACKARMINIT_MODE,END_MODE,END_MODE,END_MODE},//アーム(左下)
    {END_MODE ,      END_MODE,END_MODE         ,END_MODE  ,END_MODE        ,END_MODE,END_MODE,END_MODE},//アーム(右上)
    {END_MODE ,      END_MODE,END_MODE         ,END_MODE  ,END_MODE        ,END_MODE,END_MODE,END_MODE},//アーム(右下)
};
/* ------------------------------------------------------------------------- */
/* class宣言																 */
/* ------------------------------------------------------------------------- */
class StateManage {
    public:
        //コンストラクタ
        StateManage();
        //デストラクタ
        ~StateManage();
        //状態ゲッター
        int stateGetter(int*);
        //状態セッター
        int stateSetter(int);
        int triggerStateSetter(int);

        //状態クラスシングルトン
        static StateManage& getInstance(){
            static StateManage instance;
            return instance;
        }
    private:
    //現在の状態
       volatile int curent_state=DEVICEINIT_MODE;
};

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */