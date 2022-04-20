#include "../include/ImageInput.hpp"
#include <string>
//コンストラクタ
InputImage::InputImage(){}
//デストラクタ
InputImage::~InputImage(){}
/* -------------------------------------------------------------------------- */
/* 関数名       ：input                                                        */
/* 機能名       ：画像情報取得                                               　 */
/* 機能概要     ：画像パスから画像情報取得                                       */
/* 引数         ：char* 画像パス                                　             */
/*              : IpImage** 画像情報格納用                                     */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int InputImage::input(char* _FilePath)
{
    //引数エラー取得
    if(_FilePath==NULL){
        return SYS_PARAM;
    }
    //入力画像IpImage型宣言
    IplImage* inputfile;
    //画像データの読込（グレースケールで読込）
    //opencv上のエラーで処理の停止を防ぐ
    try{
        //画像情報をグレースケールで取得
        inputfile = cvLoadImage((const char*)_FilePath, CV_LOAD_IMAGE_GRAYSCALE);
    }catch(cv::Exception& e){
        printf("cvLoadImage:opencv上でエラーが発生");
        return SYS_NG;
    }
    imageContour(inputfile);
    cvReleaseImage(&inputfile);
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：imageSizeChange                                             */
/* 機能名       ：画像のサイズ変更                                           　 */
/* 機能概要     ：座標情報を指定サイズまで縮小させる                              */
/* 引数         ：CvSeq* 輪郭データ                             　             */
/*              : int 階層輪郭を指定するための変数                              */
/*              : IpImage** 画像情報格納用                                     */
/*              : CONTOUR_DATE* 画像座標格納用構造体                            */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int InputImage::imageSizeChange(CvSeq *_contour,int _contourcnt, IplImage* _inputimage,CONTOUR_DATE* _coordinate ){
    //計算用変数
    double calcNum=0.0f;
    double calcLargeNum=0.0f;
    double calcSmallNum=0.0f;

    //CvPointは構造体で(int x,int y)という構造をしている
    //CV_GET_SEQ_ELEM内では第三引数で指定下CvSeq内に格納された輪郭座標(CvPoint)を抜き出す役割をしている
	CvPoint *point = CV_GET_SEQ_ELEM (CvPoint, _contour,_contourcnt);

    //画像の高さ、幅が指定数値以上であるなら処理を行う
    if(_inputimage->width>BASIC_WIDTH||_inputimage->height>BASIC_HEIGHT){
        //高さより幅が大きい場合
        if(_inputimage->width<_inputimage->height){
            calcLargeNum=(double)_inputimage->height/BASIC_WIDTH;
            calcSmallNum=(double)_inputimage->width/BASIC_HEIGHT;
            if(calcLargeNum<calcSmallNum){
                calcNum=calcSmallNum;
            }else{
                calcNum=calcLargeNum;
            }
        }
        //幅より高さが大きい場合
        else{
            calcLargeNum=(double)_inputimage->width/BASIC_WIDTH;
            calcSmallNum=(double)_inputimage->height/BASIC_HEIGHT;
            if(calcLargeNum<calcSmallNum){
                calcNum=calcSmallNum;
            }else{
                calcNum=calcLargeNum;
            }
        }
        //少数点の数値拡大への調整
        calcNum+=0.01;

        //計算値を座標に反映
        _coordinate->x=((double)point->x/calcNum);
        _coordinate->y=((double)point->y/calcNum);
        //計算値が指定座標を超えた場合に走る処理
        if(_coordinate->x>BASIC_WIDTH||_coordinate->y>BASIC_HEIGHT){
            _coordinate->x=BASIC_WIDTH;
            _coordinate->y=BASIC_HEIGHT;
        }
    }else{
        _coordinate->x=(double)point->x;
        _coordinate->y=(double)point->y;
    }
    
    //アーム座標との差異を加えた数値計算
	_coordinate->x=_coordinate->x-ARM_COODINATE_WIDTH;
	_coordinate->y=_coordinate->y+ARM_COODINATE_HEIGHT;

    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：imageContour                                                 */
/* 機能名       ：画像から輪郭を取得                                          　 */
/* 機能概要     ：画像情報から、輪郭データを取得する                              */
/* 引数         : IpImage* 画像情報格納用                                       */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int InputImage::imageContour(IplImage* _inputimage){
    //引数エラー
    if(_inputimage == NULL){
        return SYS_PARAM;
    }

    //輪郭情報を格納するための宣言
    CvSeq *firstcontours = NULL;
    CvMemStorage *storage = cvCreateMemStorage (0);
    
    //白黒に二値化を行う
    cvAdaptiveThreshold (_inputimage, _inputimage,  255, CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,17,13);
    
    //輪郭取得
    cvFindContours(_inputimage,storage,&firstcontours,sizeof (CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
    
    //輪郭に内容が入っている場合
    if (firstcontours != NULL){
        //輪郭の描画
        nextContour( firstcontours, 1,_inputimage);
    }
    //最終輪郭は画像の外郭になるためここで削除
    auto it=contour_map.end();
    contour_map.erase(--it);

    //メモリストレージの解放
    cvClearSeq(firstcontours);
    cvReleaseMemStorage (&storage);
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：nextContour　                                                */
/* 機能名       ：輪郭データの詳細参照                                        　 */
/* 機能概要     ：輪郭データから座標をmap型に格納                 　              */
/* 引数         ：CvSeq* 輪郭データ                             　             */
/*              : int 階層輪郭を指定するための変数                              */
/*              : IpImage** 画像情報格納用                                     */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int InputImage::nextContour(CvSeq *_contour,int _contourcnt,IplImage* _inputimage)
{
    std::list<CONTOUR_DATE> list_temp;
    CONTOUR_DATE coordinate;
	//輪郭を構成する頂点座標を取得
	for ( int index = 0; index < _contour->total;index++){
	    imageSizeChange(_contour, index,_inputimage, &coordinate);
	    list_temp.push_back(coordinate);
	}
	contour_map[_contourcnt]=list_temp;
	if (_contour->h_next != NULL){
	    //次の輪郭がある場合は次の輪郭をmap型に格納
	    nextContour(_contour->h_next, _contourcnt+1,_inputimage);
	}
    return SYS_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名       ：contourGetter                                                */
/* 機能名       ：輪郭データの取得                                           　 */
/* 機能概要     ：輪郭データの取得                                              */
/* 引数         ：std::map<int,std::list<CONTOUR_DATE>>* 輪郭データのポインター */
/* 戻り値       ：int エラーチェック用                                          */
/* 作成日       ：1月29日、渡部湧也                                             */
/* -------------------------------------------------------------------------- */
int InputImage::contourGetter(std::map<int,std::list<CONTOUR_DATE>>* _contour_date){
    _contour_date->insert(contour_map.begin(),contour_map.end());
    return SYS_OK;
}

/* ------------------------------------------------------------------------- */
/*				Copyright HAL College of Technology & Design				 */
/* ------------------------------------------------------------------------- */