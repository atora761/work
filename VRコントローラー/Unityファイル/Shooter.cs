using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shooter : MonoBehaviour
{
    [SerializeField] GameObject bulletPrefub;//弾のプレハブ
    [SerializeField] Transform gunBarrelEnd;//発射位置

    // Update is called once per frame
    void Update()
    {
        if(Input.GetButtonDown("Fire1")) {//入力に応じて弾の発射(現在、マウスの左クリックやctrl)
            Shoot();
        }
    }

    void Shoot()
    {
        Instantiate(bulletPrefub, gunBarrelEnd.position, gunBarrelEnd.rotation);
    }
}
