using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
public class ChangeText : MonoBehaviour
{
    /* javaコードのパッケージを指定 */
    private const string PLUGIN_PACKAGE_NAME = "com.example.calculation"; 
    /* javaクラスの指定 */
    private const string JAVA_CLASS_NAME = "NativeCaluculator";
    private static AndroidJavaClass NativeCaluculator;
    public static ChangeText Instance;
    private int t_cnt =0;                                //\rの数を格納
    private int n_cnt =0;                                //\tの数を格納
    private static int axis =0;                                 //軸対応配列の索引
    private int t_first_position = 0;                    //\tの一つ目の位置を格納
    private int t_second_position = 0;                   //\tの二つ目の位置を格納
    private int n_first_position = 0;                    //\nの一つ目の位置を格納
    private int n_second_position = 0;                   //\nの二つ目の位置を格納
    private static string result;                               //最新で格納した文字列を格納
    private static string pre_result;                               //最新で格納した文字列を格納
    private static string back_result;                               //最新で格納した文字列を格納
    private static string fragmant;                           //前回オーバーしたデータを格納
    private static string current_result;                       //現在処理するデータを格納
    private static string[] change_num = new string[6];         //処理する文字列もとい数値
    private static Rigidbody rb;
    public static float startWorldPointX;
    public static float endWorldPointX;
    public static float startWorldPointY;
    public static float endWorldPointY;
    public static Camera mainCamera;
    /* 初期化処理 */
    void Start()
    {
        rb = GetComponent<Rigidbody>();
        /* javaコード呼び出し */
        Instance = this;
        NativeCaluculator = new AndroidJavaClass($"{PLUGIN_PACKAGE_NAME}.{JAVA_CLASS_NAME}");

        /* start(bluetooth受信の初期処理)メソッド呼び出し */
        NativeCaluculator.CallStatic("start");
    }

    /* 更新処理 */
    void FixedUpdate()
    {
        /* bluetoothモジュールから送られてきた値受信 */
        result = NativeCaluculator.CallStatic<string>("GetReadData");
        if (back_result == result)
        {
            NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 44行:result=null");
            return;
        }
        back_result = result;

        /* 受信した値がnullの場合処理を行わずに終了 */
        if (result == null)
        {
           NativeCaluculator.CallStatic("Unity_Log","Unity_Log 53行:result=null");
            return; 
        }
        /* \rと\nの数をカウント */
        t_cnt =CountChar(result, '\t');
        n_cnt = CountChar(result, '\n');
        /* 改行コードが複数存在する場合は多くの値を取得して古い値の場合がある為無視 */
        if (n_cnt > 2||t_cnt>2)
        {
            /* 現在の状態は不要なため破棄 */
            result = null;
            pre_result = null;
            NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 67行:複数データ入力");
            return;
        }
       
        /* \tの一つ目、二つ目の位置を検索 */
        t_first_position =result.IndexOf('\t');

        /* \nの一つ目、二つ目の位置を検索 */
        n_first_position = result.IndexOf('\n');

        while (true)
        {
            if (n_first_position == -1 && t_first_position == -1)
            {
                if (pre_result != null)
                {
                    pre_result += result;
                }
                result = null;
                return;
            }

            /* 一つ目の\nが存在しない場合二つ目も存在せず、\tだけが存在するか双方存在しないに絞れる */
            if (n_first_position == -1)
            {
                Postponed();
                NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 74行:n_first_position=-1");
                result = null;
                return;
            }

            /* 一つ目の\nは存在している\tが存在する、しないことは関係ない */
            if (t_first_position == -1)
            {
                Last_time_processing();
               NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 83行:t_first_position=-1");
                result = null;
                return;
            }

            /* 一つ目は双方存在しているため、\nが\tより大きいかの比較*/
            if (n_first_position < t_first_position)
            {
                Last_time_processing();
               NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 91行:n_first_position < t_first_position");
                break;
            }

            /* 通常処理 */
            else
            {
                Current_time_processing();
                NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 100行:通常処理");
                break;
            }
        }

        if (result != null)
        {
            pre_result += result;
        }
        /* 現在処理対象の文字列をリセット */
        result = null;

    }

    /* 終了処理 */
    private void OnDestroy()
    {
        // ヒープ領域の解放
        NativeCaluculator?.Dispose();
        NativeCaluculator = null;
        change_num = null;
    }

    /* 全体の文字列から\rを取り除いた文字数を引いて\rの数を算出して、戻り値で返す */
    private static int CountChar(string s, char c)
    {

        return (s.Length - s.Replace(c.ToString(), "").Length);
    }

    /* 値に欠陥がある場合次回に処理するようにpre_resultに格納 \tしか存在しないとき、\n,\tが存在しない場合 */
    private static void Postponed()
    {
        if (pre_result == null)
        {
            /* 前回の値と結合 */
            pre_result += result;
        }
        else
        {
            pre_result = result;
        }

    }

    /* 前回処理できなかった値を処理する \nが\tより大きい場合 \nしか存在しないとき */
    private static void Last_time_processing()
    {
        string fragmant;
        /* 処理部分だけ切り取って取得 */
        fragmant =result.Substring(0, result.IndexOf('\n')+1);
        result = result.Remove(0, result.IndexOf('\n') + 1);
        /* 取得した数値と前回取得した数値を結合 */
        pre_result += fragmant;
        fragmant = pre_result.Substring(pre_result.IndexOf('\t'), (pre_result.IndexOf('\n')- pre_result.IndexOf('\t')) + 1);
        /* 処理する部分を現在処理対象になっている文字列から削除する */
        pre_result = pre_result.Remove(pre_result.IndexOf('\t'), (pre_result.IndexOf('\n') - pre_result.IndexOf('\t')) + 1);
        /* 前回の値と結合 */
        Split_processing(fragmant);
        /* 処理部分だけ切り取って取得 */
        pre_result = null;
    }

    /* 通常処理可能な場合 \n,\tが存在するとき */
    private static void Current_time_processing()
    {
        string fragmant;
        /* 処理部分だけ切り取って取得 */
        /* result.Substring(第一\tの場所,第一\nと第一\tの差分 文字列) */
        fragmant = result.Substring(result.IndexOf('\t'), (result.IndexOf('\n')- result.IndexOf('\t'))+1);
        /* 処理する部分を現在処理対象になっている文字列から削除する */
        result = result.Remove(result.IndexOf('\t'), (result.IndexOf('\n') - result.IndexOf('\t'))+1);
        Split_processing(fragmant);

    }

    /* 処理可能な状態のデータを処理用の変数に格納していく */
    private static void Split_processing(string split_result)
    {
        if (back_result == split_result)
        {
            NativeCaluculator.CallStatic("Unity_Log", "Unity_Log 44行:result=null");
            return;
        }
        axis = 0;
        /* x座標の加速度取得 */
        /* \tと\rを処理対象から除外して値取り出し */
        current_result = split_result.Substring(split_result.IndexOf('\t')+1, ((split_result.IndexOf('\r')-1)-(split_result.IndexOf('\t') + 1))+1);
  
        /* 処理する部分を現在処理対象になっている文字列から削除する */
        /* \rから\tの不要な値を消去 */
        split_result = split_result.Remove(split_result.IndexOf('\t'), ((split_result.IndexOf('\r')-(split_result.IndexOf('\t')))-1)+1);

        if (current_result == null)
        {
           // NativeCaluculator.CallStatic("Unity_Log", "220行Unity_Log 値なし");
            return;
        }
        change_num[axis] = current_result;
        ++axis;

        /* y,z座標の加速度取得 x,y座標の角速度取得 */
        while (axis != 5)
        {
            /* y,z座標の加速度取得 x,y座標の角速度取得 */
            current_result = split_result.Substring(split_result.IndexOf('\r')+1, ((split_result.IndexOf('\r', split_result.IndexOf('\r')+1)-1 )- split_result.IndexOf('\r'))+1);
            
            /* 処理する部分を現在処理対象になっている文字列から削除する */
            split_result = split_result.Remove(split_result.IndexOf('\r'), (((split_result.IndexOf('\r', split_result.IndexOf('\r')+1))-1) - split_result.IndexOf('\r'))+1);
        
            if (current_result == null)
            {
               // NativeCaluculator.CallStatic("Unity_Log", "235行Unity_Log 値なし");
                return;
            }
            change_num[axis] = current_result;
           // NativeCaluculator.CallStatic("Unity_Log", "Unity_Log1" + change_num[axis]);
            ++axis;
        }

        /* z座標の角速度取得 */
        current_result = split_result.Substring(split_result.IndexOf('\r') + 1, ((split_result.IndexOf('\n') - 1) - (split_result.IndexOf('\r') + 1))+1);
        /* 処理する部分を現在処理対象になっている文字列から削除する */
        split_result = split_result.Remove(split_result.IndexOf('\r'), ((split_result.IndexOf('\n')+1)- split_result.IndexOf('\r')));
        
        if (current_result == null)
        {
            //NativeCaluculator.CallStatic("Unity_Log", "248行Unity_Log 値なし");
            return;
        }
        change_num[axis] = current_result;
        movement_rigid();
        axis = 0;
    }

    /* オブジェクトの移動 */
    private static void movement_rigid()
    {

        Transform cameraTrans = GameObject.Find("Sci_fi_Pistol1").transform;

        // positionを取得して各座標を+1する
        Vector3 pos = cameraTrans.position;
       
        Vector3 movement;
        double move_angle=0.0d;
        float move_x_axis = 0.0f;
        float move_y_axis = 0.0f;
        float move_z_axis = 0.0f;
       
        move_x_axis = float.Parse(change_num[0]);
        move_y_axis = float.Parse(change_num[1]);
        move_z_axis = float.Parse(change_num[2]);

        move_x_axis += 0.03f;
        move_y_axis += 0.10f;
        move_z_axis -= 0.34f;

        move_angle = Math.Atan(move_x_axis / Math.Sqrt(move_y_axis * move_y_axis + move_z_axis * move_z_axis));

        if (move_x_axis > -0.05f && 0.05f > move_x_axis)
        {
            move_x_axis = 0.0f;
        }

        mainCamera = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<Camera>();

        // Viewポートの一番左のX座標
        ChangeText.startWorldPointX = ChangeText.mainCamera.ViewportToWorldPoint(new Vector3(0.0f, 1.0f, ChangeText.mainCamera.nearClipPlane)).x;
        // Viewポートの一番右のX座標
        ChangeText.endWorldPointX = ChangeText.mainCamera.ViewportToWorldPoint(new Vector3(1.0f, 1.0f, ChangeText.mainCamera.nearClipPlane)).x;
        // Viewポートの一番上のY座標
        ChangeText.startWorldPointY = ChangeText.mainCamera.ViewportToWorldPoint(new Vector3(0.0f, 0.0f, ChangeText.mainCamera.nearClipPlane)).y;
        // Viewポートの一番下のY座標
        ChangeText.endWorldPointY = ChangeText.mainCamera.ViewportToWorldPoint(new Vector3(0.0f, 1.0f, ChangeText.mainCamera.nearClipPlane)).y;
       // NativeCaluculator.CallStatic("Unity_Log", "Unity_Log2" + "minカメラ端xy" + (ChangeText.endWorldPointX-ChangeText.startWorldPointX) + ","  + "maxカメラ端xy" +( ChangeText.startWorldPointY - ChangeText.endWorldPointY )+ "," );
        NativeCaluculator.CallStatic("Unity_Log", "Unity_Log2" + move_x_axis);
        NativeCaluculator.CallStatic("Unity_Log", "Unity_Log2" + move_z_axis);
        NativeCaluculator.CallStatic("Unity_Log", "Unity_Log2" + move_angle);
        NativeCaluculator.CallStatic("Unity_Log", "Unity_Log3" + Screen.width);
        NativeCaluculator.CallStatic("Unity_Log", "Unity_Log3" + Screen.height);
        pos.x = pos.x + ((move_x_axis*10) * Time.deltaTime);
        pos.y = pos.y + ((move_z_axis*10) * Time.deltaTime);
       // NativeCaluculator.CallStatic("Unity_Log", "Unity_Log2" + "xへの距離"+pos.x+"yへの距離"+pos.y + "zへの距離" + pos.z);
        cameraTrans.position = pos;
    }
}
