package org.wiky.lily.net;

import org.wiky.lily.LilyApplication;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by wiky on 11/15/15.
 */
public class Server implements Runnable {

    private ServerSocket mSocket;
    private boolean mRunning = false;

    public Server() {
        try {
            mSocket = new ServerSocket(LilyApplication.mAdbConfig.port);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void runInThread() {
        mRunning = true;
        new Thread(this).start();
    }

    public void stopThread() {
        try {
            mSocket.close();
            mRunning = false;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            while (mRunning) {
                Socket socket = mSocket.accept();
                if (socket != null) {
                    new Client(socket).start();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
