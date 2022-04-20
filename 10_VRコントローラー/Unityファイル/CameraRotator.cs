using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraRotator : MonoBehaviour
{
    [SerializeField] float angularVelocity = 30f;
    float horizontalAngle = 0f;
    float verticalAngle = 0f;
#if UNITY_EDITOR//スマホでの確認では使われずに、unity上でしか動かないようにする分岐
    void Update() {
        //入力による回転量を取得
        var horizontalRotation = Input.GetAxis("Horizontal") * angularVelocity * Time.deltaTime;//変位値*回転速度*前フレームからの経過時間
        var verticalRatation = -Input.GetAxis("Vertical") * angularVelocity * Time.deltaTime;
        //回転量を更新
        horizontalAngle += horizontalRotation;
        verticalAngle += verticalRatation;
        //垂直方向は回転しすぎないように制限
        verticalAngle = Mathf.Clamp(verticalAngle, -80f, 80f);
        //Transformコンポーネントに回転量を適用する
        transform.rotation = Quaternion.Euler(verticalAngle,horizontalAngle, 0f);
    }
#endif
}
