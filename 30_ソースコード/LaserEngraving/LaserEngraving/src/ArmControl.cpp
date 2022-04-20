/* ------------------------------------------------------------------------- */
/* ArmControl.cpp															 */
/*																			 */
/*																			 */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		 */
/* ------------------------------------------------------------------------- */
/* 000000	新規作成								2022/01/27	崎山　勇人	 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* includeファイル															 */
/* ------------------------------------------------------------------------- */
#include "../include/ArmControl.hpp"

/* ------------------------------------------------------------------------- */
/* 関数名		： ArmControl												 */
/* 機能名		： コンストラクタ											 */
/* 機能概要		： メンバー変数の初期化										 */
/* 引数			： なし														 */
/* 戻り値		： 															 */
/* 作成日		： 2022/02/27		 崎山　勇人		 新規作成			 	 */
/* ------------------------------------------------------------------------- */
ArmControl::ArmControl()
{
	/* ステッピングモータ制御のインスタンス */
	FixedMotor &fixedMotor = FixedMotor::getInstance();
	MiddleMotor &middleMotor = MiddleMotor::getInstance();
	/* 初期化 ---------------------------------------------------------------- */
	/* アームの長さ */
	length.l1 = 100;
	length.l2 = 100;
	/* ステッピングモータの角度 */
	degree.deg1 = 0;
	degree.deg2 = 0;
	/* 初期化コマンド送信処理 */
	fixedMotor.init();
	middleMotor.init();
}
ArmControl::~ArmControl() {}

/* ------------------------------------------------------------------------- */
/* 関数名		： stop														 */
/* 機能名		： 停止														 */
/* 機能概要		： 両方のモータを止める										 */
/* 引数			： なし														 */
/* 戻り値		： int				:0				:正常終了				 */
/* 作成日		： 2022/02/27		 崎山　勇人		 新規作成			 	 */
/* ------------------------------------------------------------------------- */
int ArmControl::stop()
{
	/* ステッピングモータ制御のインスタンス */
	FixedMotor &fixedMotor = FixedMotor::getInstance();
	MiddleMotor &middleMotor = MiddleMotor::getInstance();
	/* モータを止めるコマンドを送信 */
	fixedMotor.stop();
	middleMotor.stop();

	return 1;
}

/* ------------------------------------------------------------------------- */
/* 関数名		： manualMode														 */
/* 機能名		： マニュアルモード														 */
/* 機能概要		： モータと方向を選択して、一ステップずつ動かす										 */
/* 引数			： int				:dir				:方向					 */
/*				： int				:select				:モータ選択				 */
/* 戻り値		： int				:SYS_OK				:正常終了				 */
/*				： int				:SYS_PARAM			:引数エラー			 */
/* 作成日		： 2022/02/27		 崎山　勇人		 新規作成			 	 */
/* ------------------------------------------------------------------------- */
int ArmControl::manualMode(int dir, char select)
{
	/* エラーチェック ------------------------------------------------- */
	/* 方向値が1か0以外の値の場合エラー */
	if (dir > 1 || dir < 0)
	{
		return SYS_PARAM;
	}
	if (select > BACK || select < FRONT )
	{
		return SYS_PARAM;
	}

	/* ステッピングモータ制御のインスタンス */
	MiddleMotor &middleMotor = MiddleMotor::getInstance();
	FixedMotor &fixedMotor = FixedMotor::getInstance();

	/* selectの値によって駆動するモータを選ぶ */
	/* 1ステップずつモータを駆動させる */
	if (select == FRONT)
	{
		middleMotor.run(1, dir);
	}
	else
	{
		fixedMotor.run(1, dir);
	}
	delay(2);

	return SYS_OK;
}

/* ------------------------------------------------------------------------- */
/* 関数名		： run														 */
/* 機能名		： 実行														 */
/* 機能概要		： 逆運動計算を行い各角度でモータを動かす						 */
/* 引数			： なし														 */
/* 戻り値		： int				:0				:正常終了				 */
/* 作成日		： 2022/02/27		 崎山　勇人		 新規作成			 	 */
/* ------------------------------------------------------------------------- */
int ArmControl::run(double x, double y)
{
	/* ステッピングモータ制御のインスタンス */
	FixedMotor &fixedMotor = FixedMotor::getInstance();
	MiddleMotor &middleMotor = MiddleMotor::getInstance();
	
	/* 変数宣言 */
	POSITION pos;
	pos.x = x;
	pos.y = y;
	// 逆運動学で座標から各ラジアンを取得
	THETA t = calc(pos);
	// ラジアンから角度に変換
	double degree0 = t.th1 * 180.0 / PI;
	double degree1 = t.th2 * 180.0 / PI;
	int step1 = 0;
	int step2 = 0;
	int dir1 = 0;
	int dir2 = 0;
	int max = 0;

	if((degree.deg1+(degree0 - degree.deg1))>=350.0f)
	{
		
	}

	/* 角度からstep数を取得 */
	step1 = (int)round((degree0 - degree.deg1) / ONE_MICRO_STEP);
	step2 = (int)round((degree1 - degree.deg2) / ONE_MICRO_STEP);
	(step1 < 0) ? step1 = -step1 : step1;
	(step2 < 0) ? step2 = -step2 : step2;

	/* 方向を調べる */
	(degree0 - degree.deg1 > 0) ? dir1 = 1 : dir1 = 0;
	(degree1 - degree.deg2 > 0) ? dir2 = 1 : dir2 = 0;
	degree.deg1 = degree0;
	degree.deg2 = degree1;

	/* ステップ数の最大値を取得 */
	max = step1;
	(max < step2) ? max = step2 : max;

	/* 一ステップずつ実行 */
	for (int i = 0; i < max; i++)
	{
		if (i < step1)
		{
			fixedMotor.run(1, dir1);
		}
		if (i < step2)
		{
			middleMotor.run(1, dir2);
		}
		delay(2);
	}

	return 1;
}

/* ------------------------------------------------------------------------- */
/* 関数名		： calc														 */
/* 機能名		： 逆運動													 */
/* 機能概要		： 逆運動計算を行う											 */
/* 引数			： POSITION			:pos			:座標					 */
/* 戻り値		： THETA			:T				:計算結果				 */
/* 作成日		： 2022/02/27		 崎山　勇人		 新規作成			 	 */
/* ------------------------------------------------------------------------- */
THETA ArmControl::calc(POSITION pos)
{
	/* 変数宣言 */
	double x = pos.x;
	double y = pos.y;
	double l1 = length.l1;
	double l2 = length.l2;
	THETA T;
	double temp;

	double norm = sqrt((pos.x * pos.x) + (pos.y * pos.y));
	/* 逆運動計算 */

	T.th1 = atan2(y, x) - acos(((x * x) + (y * y) + (l1 * l1) - (l1 * l2)) / (2 * l1 * norm));
	temp = atan2(y, x) + acos(((x * x) + (y * y) + (l1 * l1) - (l1 * l2)) / (2 * l1 * norm));
	if(T.th1>temp){
		T.th1=temp;
	}

	T.th2 = -T.th1 + atan2((y - l1 * sin(T.th1)), (x - l1 * cos(T.th1)));

	return T;
}

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */