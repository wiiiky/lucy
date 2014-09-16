package org.wl.ama;

import android.util.Log;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by wiky on 9/16/14.
 *
 * 处理单个链接的线程
 */
public class ConnectionThread extends Thread {
    /* 处理一个链接的线程，目前只是echo */
    private Socket socket=null;
    public ConnectionThread(Socket s){
        socket=s;
    }

    public void run(){
        try {
            InputStreamReader inputReader=new InputStreamReader(socket.getInputStream());
            PrintWriter printWriter=new PrintWriter(socket.getOutputStream());
            char[] buf=new char[4096];
            while(inputReader.read(buf)>0){
                printWriter.write(buf);
                printWriter.flush();
            }
        } catch (IOException e) {
            Log.e("IOException", e.getMessage() != null ? e.getMessage() : "!");
        }finally {
            try {
                socket.close();
            } catch (IOException e) {
            }
        }
    }
}
