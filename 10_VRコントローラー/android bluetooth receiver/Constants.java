package com.example.ohs90774.myapplication;

import java.util.UUID;

public interface Constants {
    public static final String BT_DEVICE = "DSD TECH HC-05";
    public static final UUID BT_UUID = UUID.fromString(
            "00001101-0000-1000-8000-00805f9b34fb");

    public static final String STATE_TEMP = "STATE_TEMP";

    public static final int MESSAGE_BT = 0;
    public static final int MESSAGE_TEMP = 2;
}