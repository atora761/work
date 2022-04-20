package com.example.ohs90774.myapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Set;
import java.util.UUID;

import static android.content.ContentValues.TAG;

public class MainActivity extends AppCompatActivity  {
    Handler h;
    static final int RECIEVE_MESSAGE = 1;
    private String deviceAddress;
    BTClientThread btClientThread;
    private StringBuilder sb = new StringBuilder();
    EditText temperatureText;
    private final static int CONNECTING_STATUS = 1; // used in bluetooth handler to identify message status
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update
    BluetoothAdapter bluetoothAdapter;
    TextView tempTextView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Find Views
        tempTextView = (TextView) findViewById(R.id.readdata);

        if(savedInstanceState != null){
            String temp = savedInstanceState.getString(Constants.STATE_TEMP);
            tempTextView.setText(temp);
        }

        // Initialize Bluetooth
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if( bluetoothAdapter == null ){
            Log.d(TAG, "This device doesn't support Bluetooth.");
        }
    }
    @Override
    protected void onResume() {
        super.onResume();
        btClientThread = new BTClientThread();
        btClientThread.start();
    }

    @Override
    protected void onPause(){
        super.onPause();
        if(btClientThread != null){
            btClientThread.interrupt();
            btClientThread = null;
        }
    }
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString(Constants.STATE_TEMP, tempTextView.getText().toString());
    }

    public class BTClientThread extends Thread {

        InputStream inputStream;
        OutputStream outputStrem;
        BluetoothSocket bluetoothSocket;

        public void run() {

            byte[] incomingBuff = new byte[64];

            BluetoothDevice bluetoothDevice = null;
            Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();
            for(BluetoothDevice device : devices){
                if(device.getName().equals(Constants.BT_DEVICE)) {
                    bluetoothDevice = device;
                    break;
                }
            }

            if(bluetoothDevice == null){
                Log.d(TAG, "No device found.");
                return;
            }

            try {

                bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(
                        Constants.BT_UUID);

                while(true) {

                    if(Thread.interrupted()){
                        break;
                    }

                    try {
                        bluetoothSocket.connect();

                        handler.obtainMessage(
                                Constants.MESSAGE_BT,
                                "CONNECTED " + bluetoothDevice.getName())
                                .sendToTarget();

                        inputStream = bluetoothSocket.getInputStream();
                        outputStrem = bluetoothSocket.getOutputStream();

                        while (true) {

                            if (Thread.interrupted()) {
                                break;
                            }

                            // Send Command
                            String command = "GET:TEMP";
                            outputStrem.write(command.getBytes());
                            // Read Response
                            int incomingBytes = inputStream.read(incomingBuff);
                            byte[] buff = new byte[incomingBytes];
                            System.arraycopy(incomingBuff, 0, buff, 0, incomingBytes);
                            String s = new String(buff, StandardCharsets.UTF_8);

                            // Show Result to UI
                            handler.obtainMessage(
                                    Constants.MESSAGE_TEMP,
                                    s)
                                    .sendToTarget();

                            // Update again in a few seconds
                            Thread.sleep(3000);
                        }

                    } catch (IOException e) {
                        // connect will throw IOException immediately
                        // when it's disconnected.
                        Log.d(TAG, e.getMessage());
                    }

                    handler.obtainMessage(
                            Constants.MESSAGE_BT,
                            "DISCONNECTED")
                            .sendToTarget();

                    // Re-try after 3 sec
                    Thread.sleep(3 * 1000);
                }

            }catch (InterruptedException e){
                e.printStackTrace();
            }
            catch (IOException e) {
                e.printStackTrace();
            }

            if(bluetoothSocket != null){
                try {
                    bluetoothSocket.close();
                } catch (IOException e) {}
                bluetoothSocket = null;
            }

            handler.obtainMessage(
                    Constants.MESSAGE_BT,
                    "DISCONNECTED - Exit BTClientThread")
                    .sendToTarget();
        }
    }
    final Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {

            String s;

            switch (msg.what) {
                case Constants.MESSAGE_BT:
                    s = ( String ) msg.obj;

                    break;
                case Constants.MESSAGE_TEMP:
                    s = ( String ) msg.obj;
                    if (s != null) {
                        tempTextView.setText(s);
                    }
                    break;
            }
        }
    };
}