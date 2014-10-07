package org.wl.ll;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ListenService extends Service {
    private static int LISTEN_PORT =37859;
    private ServerSocket serverSocket=null;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate(){
        super.onCreate();

        Runnable runnable=new Runnable() {
            @Override
            public void run() {
                Socket client;
                try{
                    serverSocket=new ServerSocket(LISTEN_PORT);
                    while((client = serverSocket.accept())!=null){
                        new ConnectionThread(ListenService.this,client).start();
                    }
                }catch (Exception e) {
                }
            }
        };

        new Thread(runnable).start();

    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        if(serverSocket!=null) {
            try {
                serverSocket.close();
            } catch (IOException e) {
            }
        }
    }
}
