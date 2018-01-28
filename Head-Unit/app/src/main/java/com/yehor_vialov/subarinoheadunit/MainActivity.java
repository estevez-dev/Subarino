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
import android.widget.Switch;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;
import java.util.logging.Logger;

public class MainActivity extends AppCompatActivity {

    private final static int REQUEST_ENABLE_BT = 1;
    Button btnConnect;
    OutputStream btOutStream;
    InputStream btInStream;
    BluetoothSocket btSocket = null;
    Switch swHLLowBeam;
    Switch swHLAuto;
    Switch swHLParking;
    TextView txtBTStatus;
    boolean ignoreSwitchChange = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        txtBTStatus = (TextView)findViewById(R.id.txtBTStatus);
        swHLLowBeam = (Switch)findViewById(R.id.swHLLowBeam);
        swHLAuto = (Switch)findViewById(R.id.swHLAuto);
        swHLParking = (Switch)findViewById(R.id.swHLParking);

        switchHLControls(false);
        connect();
        setListeners();
    }

    @Override
    protected void onDestroy() {
        disconnect();
        super.onDestroy();
    }

    private void disconnect() {
        try {
            btOutStream.close();
            btInStream.close();
        } catch (Exception e) {
            //e.printStackTrace();
        }
        try {
            btSocket.close();
        } catch (Exception e) {
            //e.printStackTrace();
        }
        switchHLControls(false);
    }

    private void connect() {
        new Thread() {
            public void run() {
                boolean enableBTRequested = false;
                while (btSocket==null || !btSocket.isConnected()) {
                    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
                    if (adapter == null) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                txtBTStatus.setText("Bluetooth is not supported");
                            }
                        });
                    } else {
                        if (!adapter.isEnabled() && !enableBTRequested) {
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    txtBTStatus.setText("Bluetooth is not enabled. Will retry in 5 sec.");
                                }
                            });
                            //make sure the device's bluetooth is enabled
                            enableBTRequested = true;
                            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                            startActivityForResult(enableBluetooth, REQUEST_ENABLE_BT);
                        } else if (adapter.isEnabled()) {
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    switchHLControls(false);
                                    txtBTStatus.setText("Connecting...");
                                }
                            });
                            enableBTRequested = false;
                            final UUID SERIAL_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); //UUID for serial connection
                            String mac = "00:21:13:03:39:FE"; //HC-05
                            BluetoothDevice device = adapter.getRemoteDevice(mac); //get remote device by mac, we assume these two devices are already paired
                            // Get a BluetoothSocket to connect with the given BluetoothDevice
                            btSocket = null;
                            btOutStream = null;
                            btInStream = null;
                            try {
                                btSocket = device.createRfcommSocketToServiceRecord(SERIAL_UUID);
                            } catch (Exception e) {
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        txtBTStatus.setText("Error creating socket.  Will retry in 5 sec.");
                                    }
                                });
                            }

                            try {
                                btSocket.connect();
                                btOutStream = btSocket.getOutputStream();
                                btInStream = btSocket.getInputStream();

                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        switchHLControls(true);
                                        txtBTStatus.setText("Loading settings...");
                                        reader();

                                    }
                                });
                                sendCommand("LS0");
                                while (btInStream.available() == 0) {
                                    try { Thread.sleep(100); } catch (InterruptedException e) { e.printStackTrace(); }
                                }


                                final char parkingLights = (char)btInStream.read();
                                final char lowbeamLights = (char)btInStream.read();
                                final char lowbeamAuto = (char)btInStream.read();

                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        switchHLControls(true);
                                        ignoreSwitchChange = true;
                                        if (parkingLights == '0') swHLParking.setChecked(false); else swHLParking.setChecked(true);
                                        if (lowbeamLights == '0') swHLLowBeam.setChecked(false); else swHLLowBeam.setChecked(true);
                                        if (lowbeamAuto == '0') swHLAuto.setChecked(false); else swHLAuto.setChecked(true);
                                        ignoreSwitchChange = false;
                                        txtBTStatus.setText("Connected.");
                                        reader();
                                    }
                                });



                            } catch (Exception e) {
                                final Exception er = e;
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        txtBTStatus.setText("Connection error. Will retry in 5 sec.");
                                    }

                                });
                                e.printStackTrace();
                            }
                        }
                    }
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }.start();
    }

    private void reader() {
        new Thread() {
            public void run() {
                while (btSocket!=null && btSocket.isConnected()) {
                    try {
                        if (btInStream.available() > 0) {
                            final char parkingLights = (char)btInStream.read();
                            final char lowbeamLights = (char)btInStream.read();
                            final char lowbeamAuto = (char)btInStream.read();

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    ignoreSwitchChange = true;
                                    if (parkingLights == '0') swHLParking.setChecked(false); else swHLParking.setChecked(true);
                                    if (lowbeamLights == '0') swHLLowBeam.setChecked(false); else swHLLowBeam.setChecked(true);
                                    if (lowbeamAuto == '0') swHLAuto.setChecked(false); else swHLAuto.setChecked(true);
                                    ignoreSwitchChange = false;
                                }
                            });
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        Thread.sleep(200);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

            }
        }.start();
    }

    private void switchHLControls(boolean enabled) {
        swHLAuto.setEnabled(enabled);
        swHLLowBeam.setEnabled(enabled);
        swHLParking.setEnabled(enabled);
    }

    private void sendCommand(String command) {
        try {
            btOutStream.write(command.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
            txtBTStatus.setText("Disconnected");
            disconnect();
            connect();
        }
    }

    private void setListeners() {
        swHLAuto.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (!ignoreSwitchChange) {
                    if(isChecked) sendCommand("LA1"); else sendCommand("LA0");
                }

            }
        });


        swHLLowBeam.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (!ignoreSwitchChange) {
                    if(isChecked) sendCommand("LL1"); else sendCommand("LL0");
                }

            }
        });

        swHLParking.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (!ignoreSwitchChange) {
                    if(isChecked) sendCommand("LP1"); else sendCommand("LP0");
                }

            }
        });
    }
}
