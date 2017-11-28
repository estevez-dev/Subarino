package com.yehor_vialov.subarinoheadunit;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private final static int REQUEST_ENABLE_BT = 1;
    Button btnConnect;
    OutputStream btOutStream = null;
    Boolean btConnected = false;
    ToggleButton btnLed;
    BluetoothSocket btSocket = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnConnect = (Button)findViewById(R.id.btnConnect);
        btnLed = (ToggleButton)findViewById(R.id.btnLed);
        btnLed.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked)
                {
                    try {
                        btOutStream.write("On".getBytes());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else
                {
                    try {
                        btOutStream.write("Off".getBytes());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        btnConnect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if (!btConnected) {
                    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
                    if (adapter == null) {
                        // Device does not support Bluetooth
                        finish(); //exit
                    }

                    if (!adapter.isEnabled()) {
                        //make sure the device's bluetooth is enabled
                        Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(enableBluetooth, REQUEST_ENABLE_BT);
                    }

                    final UUID SERIAL_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //UUID for serial connection
                    String mac = "00:21:13:03:39:FE"; //my laptop's mac adress
                    BluetoothDevice device = adapter.getRemoteDevice(mac); //get remote device by mac, we assume these two devices are already paired


                    // Get a BluetoothSocket to connect with the given BluetoothDevice
                    btSocket = null;
                    btOutStream = null;
                    try {
                        btSocket = device.createRfcommSocketToServiceRecord(SERIAL_UUID);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    try {
                        btSocket.connect();
                        btOutStream = btSocket.getOutputStream();
                        btConnected = true;
                        btnConnect.setText("Disconnect");
                        //now you can use out to send output via out.write
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else {
                    try {
                        btOutStream.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        btSocket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    btConnected = false;
                    btnConnect.setText("Connect");
                }
            }
        });
    }
}
