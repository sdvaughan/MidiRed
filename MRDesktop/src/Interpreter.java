import java.io.*;
import java.net.*;


public class Interpreter {

    public static void main(String[] args) throws Exception {
        Socket inSocket;
        try {
             inSocket = new Socket(args[0], Integer.parseInt(args[1]) + 1);
        } catch (Exception e){
            throw new Exception("invalid port number");
        }//end try catch

        DataInputStream inputStream = new DataInputStream(inSocket.getInputStream());

        byte[] block = new byte[4];

        while (true){
            for(int i = 0; i<4; i++){
                block[i] = inputStream.readByte();
            }//end for

        }//end while

    }//end main

}//end interpreter
