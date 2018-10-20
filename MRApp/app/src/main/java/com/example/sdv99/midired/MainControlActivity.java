package com.example.sdv99.midired;


import android.app.Application;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.media.midi.*;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class MainControlActivity extends AppCompatActivity {

    MidiManager midiMan;
    MidiInputPort inputPort;
    private int activity_main_control;
    boolean deviceOpened = false;
    DataOutputStream outputStream;
    byte[] block = new byte[4];
    static Semaphore semaphore = new Semaphore(1);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_control);

        final ListView midiConnList = (ListView)findViewById(R.id.listView);

        midiMan = (MidiManager)this.getSystemService(Context.MIDI_SERVICE);
        final MidiDeviceInfo[] midiInfos = midiMan.getDevices();
        String[] midiNames = new String[midiInfos.length];


        for (int i = 0; i < midiInfos.length; i++) {
            midiNames[i] = (String) midiInfos[i].getProperties().get(MidiDeviceInfo.PROPERTY_NAME);
        }

        AlertDialog chooseDevice = new AlertDialog.Builder(this).create();

        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                Socket outSocket;
                try {
                    outSocket = new Socket("69.174.144.193", 7781);
                    outputStream = new DataOutputStream(outSocket.getOutputStream());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }//end try catch
            }
        });

        thread.start();


    /*    if (midiInfos.length > 0){
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setTitle("Pick a MIDI device");
                builder.setItems(midiNames, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        final int usedDeviceInt = which;
                        midiMan.openDevice(midiInfos[which], new MidiManager.OnDeviceOpenedListener() {
                                    @Override
                                    public void onDeviceOpened(MidiDevice device) {
                                        inputPort = device.openInputPort(0);
                                        deviceOpened = true;
                                    }
                                }, new Handler(Looper.getMainLooper())
                        );
                    }

                });
                chooseDevice = builder.create();
                //chooseDevice.show(
        }
     */
        if (midiInfos.length > 0) {
            midiMan.openDevice(midiInfos[0], new MidiManager.OnDeviceOpenedListener() {
                        @Override
                        public void onDeviceOpened(MidiDevice device) {
                            inputPort = device.openInputPort(0);
                        }
                    }, new Handler(Looper.getMainLooper())
            );

        }

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, midiNames);
        midiConnList.setAdapter(adapter);

        final TextView firstAmtText = (TextView)findViewById(R.id.textView);
        final TextView secondAmtText = (TextView)findViewById(R.id.textView2);
        final TextView thirdAmtText = (TextView)findViewById(R.id.textView3);
        final TextView fourthAmtText = (TextView)findViewById(R.id.textView4);
        final SeekBar firstSlider = (SeekBar)findViewById(R.id.seekBar);
        firstSlider.setMax(127);
        firstSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                firstAmtText.setText(Integer.toString(progress * 100 / 127) + "%");


                block[1] = 0x0; // MIDI channels 1-16 are encoded as 0-15.
                block[0] = (byte)(0x90); // note on
                block[2] = (byte)(0x07); // pitch is middle C
                block[3] = (byte)progress; // max velocity
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            semaphore.acquire();
                            for (int i = 0; i < 4; i++) {

                                //Log.d("FIRSTSLIDER",String.valueOf(block[i]));
                                outputStream.writeByte(block[i]);
                            }

                            semaphore.release();
                        } catch (Exception e){
                            throw new RuntimeException("Couldn't send message");
                        }
                    }
                });

                thread.start();
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar){}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar){}
        });

        final SeekBar secondSlider = (SeekBar)findViewById(R.id.seekBar2);
        secondSlider.setMax(127);
        secondSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                secondAmtText.setText(Integer.toString(progress * 100 / 127) + "%");

                block[1] = 0x1; // MIDI channels 1-16 are encoded as 0-15.
                block[0] = (byte)(0x90); // note on
                block[2] = (byte)(0x07); // pitch is middle C
                block[3] = (byte)progress; // max velocity
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            semaphore.acquire();
                            for (int i = 0; i < 4; i++) {
                                //Log.d("SECONDSLIDER",String.valueOf(block[i]));
                                outputStream.writeByte(block[i]);
                            }
                            semaphore.release();
                        } catch (Exception e){
                            throw new RuntimeException("Couldn't send message");
                        }
                    }
                });

                thread.start();
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar){}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar){}
        });

        final SeekBar thirdSlider = (SeekBar)findViewById(R.id.seekBar3);
        thirdSlider.setMax(127);
        thirdSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                thirdAmtText.setText(Integer.toString(progress * 100 / 127) + "%");

                block[1] = 0x2; // MIDI channels 1-16 are encoded as 0-15.
                block[0] = (byte)(0x90); // note on
                block[2] = (byte)(0x07); // pitch is middle C
                block[3] = (byte)progress; // max velocity
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            semaphore.acquire();
                            for (int i = 0; i < 4; i++) {
                                //Log.d("THIRDSLIDER",String.valueOf(block[i]));
                                outputStream.writeByte(block[i]);
                            }
                            semaphore.release();
                        } catch (Exception e){
                            throw new RuntimeException("Couldn't send message");
                        }
                    }
                });

                thread.start();
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar){}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar){}
        });

        final SeekBar fourthSlider = (SeekBar)findViewById(R.id.seekBar4);
        fourthSlider.setMax(127);
        fourthSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                fourthAmtText.setText(Integer.toString(progress * 100 / 127) + "%");

                block[1] = 0x3; // MIDI channels 1-16 are encoded as 0-15.
                block[0] = (byte)(0x90); // note on
                block[2] = (byte)(0x07); // pitch is middle C
                block[3] = (byte)progress; // max velocity
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            semaphore.acquire();
                            for (int i = 0; i < 4; i++) {
                                //Log.d("FOURTHSLIDER",String.valueOf(block[i]));
                                outputStream.writeByte(block[i]);
                            }
                            semaphore.release();
                        } catch (Exception e){
                            throw new RuntimeException("Couldn't send message");
                        }
                    }
                });

                thread.start();
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar){}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar){}
        });

        firstAmtText.setText(Integer.toString(firstSlider.getProgress() * 100 / 127) + "%");
        secondAmtText.setText(Integer.toString(secondSlider.getProgress() * 100 / 127)  + "%");
        thirdAmtText.setText(Integer.toString(thirdSlider.getProgress() * 100 / 127) + "%");
        fourthAmtText.setText(Integer.toString(fourthSlider.getProgress() * 100 / 127) + "%");

    }
}
