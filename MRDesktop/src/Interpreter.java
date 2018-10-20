import java.io.*;
import java.net.*;
import javax.sound.midi.*;


public class Interpreter {

    public static void main(String[] args) throws Exception {
        Socket inSocket;
        if (args.length != 2) {
            System.out.println("Usage:\nInterpreter <host> <port>");
        }
        try {
             inSocket = new Socket(args[0], Integer.parseInt(args[1]) + 1);
        } catch (Exception e){
            throw new Exception("Invalid arguments.");
        }//end try catch

        DataInputStream inputStream = new DataInputStream(inSocket.getInputStream());

        byte[] block = new byte[4];

        MidiDevice device1 = null;
        MidiDevice device2 = null;
        MidiDevice.Info[] infos = MidiSystem.getMidiDeviceInfo();
        boolean dev1Found = false;

        for (int i = 0; i < infos.length; i++) {
            if (infos[i].getName().equals("LoopBe Internal MIDI")){
                if (!dev1Found){
                    device1 = MidiSystem.getMidiDevice(infos[i]);
                    dev1Found = true;
                } else {
                    device2 = MidiSystem.getMidiDevice(infos[i]);
                }
            }
        }

        Receiver receiver;

        if (device1.getMaxReceivers() == 0) {
            receiver = device1.getReceiver();
        } else {
            receiver = device2.getReceiver();
        }

        while (true){
            for(int i = 0; i<4; i++){
                block[i] = inputStream.readByte();
            }//end for

            ShortMessage myMsg = new ShortMessage();
            myMsg.setMessage(block[0], block[1], block[2], block[3]);
            long timeStamp = -1;
            receiver.send(myMsg, timeStamp);

        }//end while

    }//end main

}//end interpreter
