package org.wiky.lily.net;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Created by wiky on 11/15/15.
 */
public class Client implements Runnable {

    private Socket mSocket;

    public Client(Socket socket) {
        mSocket = socket;
    }

    public void start() {
        new Thread(this).start();
    }

    @Override
    public void run() {
        try {
            InputStream inputStream = mSocket.getInputStream();
            OutputStream outputStream = mSocket.getOutputStream();

            while (true) {
                byte[] lenBuf = new byte[4];
                if (inputStream.read(lenBuf) != 4) {
                    break;
                }
                int length = parseLength(lenBuf);
                byte[] payloadBuf = new byte[length];
                if (inputStream.read(payloadBuf) != length) {
                    break;
                }

                outputStream.write(pack(payloadBuf));
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private int parseLength(byte[] data) {
        int length = 0;
        for (int i = 0; i < 4; i++) {
            length += data[i] << ((3 - i) * 8);
        }
        return length;
    }

    private byte[] pack(byte[] data) {
        int length = data.length;
        byte[] buf = new byte[4 + length];
        byte[] len = new byte[4];
        len[0] = (byte) (length >> 24);
        len[1] = (byte) (length >> 16);
        len[2] = (byte) (length >> 8);
        len[3] = (byte) (length);
        System.arraycopy(len, 0, buf, 0, len.length);
        System.arraycopy(data, 0, buf, len.length, data.length);
        return buf;
    }
}
