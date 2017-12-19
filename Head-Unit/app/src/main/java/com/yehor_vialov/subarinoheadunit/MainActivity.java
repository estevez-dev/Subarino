package com.yehor_vialov.subarinoheadunit;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.Switch;
import android.widget.ToggleButton;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private final static int REQUEST_ENABLE_BT = 1;
    Button btnConnect;
    OutputStream btOutStream;
    Boolean btConnected = false;
    BluetoothSocket btSocket = null;
    Switch swHeadlights = null;
    Switch swHLAuto = null;
    ImageButton btnHLR = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        btnHLR = (ImageButton)findViewById(R.id.btnHLR);

        btnHLR.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ((ImageButton)v).setVisibility(View.INVISIBLE);
            }
        });

        /*btnConnect = (Button)findViewById(R.id.btnConnect);

        swHeadlights = (Switch)findViewById(R.id.swHeadlights);
        swHLAuto = (Switch)findViewById(R.id.swHLAuto);
        swHLAuto.setChecked(true);
        switchHLControls(false);
        swHLAuto.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked)
                {
                    try {
                        btOutStream.write("13".getBytes());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else
                {
                    try {
                        btOutStream.write("14".getBytes());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });


        swHeadlights.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked)
                {
                    try {
                        btOutStream.write("11".getBytes());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                else
                {
                    try {
                        btOutStream.write("12".getBytes());
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
                        switchHLControls(true);
                        //now you can use out to send output via out.write
                    } catch (IOException e) {
                        switchHLControls(false);
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
                    switchHLControls(false);
                    btConnected = false;
                    btnConnect.setText("Connect");
                }
            }
        });*/
    }

    private void switchHLControls(boolean enabled) {
        swHLAuto.setEnabled(enabled);
        swHeadlights.setEnabled(enabled);
    }
}
