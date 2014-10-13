package org.wl.ll;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

import org.wl.ll.protocol.*;

/**
 * Created by wiky on 9/16/14.
 * <p/>
 * 处理单个链接的线程
 */
public class ConnectionThread extends Thread {
    /* 处理一个链接的线程 */
    private Socket socket = null;
    private OutputStream outputStream = null;
    private BufferedReader bufferedReader = null;
    private Context mContext = null;

    /*
     * 请求数据
     */
    private static String REQUEST_PACKAGES = "applications";  /* 应用列表 */
    private static String REQUEST_ICON = "icon:";     /* 应用图标 */
    private static String REQUEST_VERSION = "version";    /* 当前手机客户端的版本号 */
    private static String REQUEST_PHONE = "phone";    /* 手机的基本信息 */

    public ConnectionThread(Context ctx, Socket s) {
        socket = s;
        mContext = ctx;

        MainActivity.LOG(socket.getRemoteSocketAddress().toString() + " is connected!!!");
    }

    public void run() {
        try {
            InputStreamReader inputReader = new InputStreamReader(socket.getInputStream());
            outputStream = socket.getOutputStream();
            bufferedReader = new BufferedReader(inputReader);
            String buf;
            while ((buf = bufferedReader.readLine()) != null) {
                String lower = buf.toLowerCase();
                MainActivity.LOG(socket.getRemoteSocketAddress().toString() + ":" + buf);
                if (lower.equals(REQUEST_PACKAGES)) {
                    onApplicationsResponse();
                } else if (lower.startsWith(REQUEST_ICON)) {
                    onIconResponse(buf.substring(REQUEST_ICON.length()));
                } else if (lower.equals(REQUEST_VERSION)) {
                    onVersionResponse();
                } else if (lower.equals(REQUEST_PHONE)) {
                    onPhoneResponse();
                } else {
                    onUnknownResponse(buf);
                }
            }
        } catch (IOException e) {
            MainActivity.LOG(e.getMessage() != null ? e.getMessage() : "!");
        } finally {
            MainActivity.LOG(socket.getRemoteSocketAddress().toString() + " is disconnected!!!");
            cleanup();
        }
    }

    private void cleanup() {
        try {
            if (outputStream != null) {
                outputStream.close();
            }
            if (bufferedReader != null) {
                bufferedReader.close();
            }
        } catch (IOException e) {
        }
        try {
            socket.close();
        } catch (IOException e) {
        }
    }

    private void onApplicationsResponse() {
        new ApplicationsResponse(mContext).onResponse(outputStream);
    }

    private void onVersionResponse() {
        new VersionResponse(mContext).onResponse(outputStream);
    }

    private void onPhoneResponse() {
        new PhoneResponse(mContext).onResponse(outputStream);
    }

    private void onIconResponse(String packageName) {
        new IconResponse(mContext, packageName).onResponse(outputStream);
    }

    /*
     * 未知的请求
     * 直接返回该请求数据
     */
    private void onUnknownResponse(String buf) {
        new UnknownResponse(mContext, buf).onResponse(outputStream);
    }
}
