package org.wl.ll;

import android.content.Context;

import org.wl.ll.protocol.ApplicationsResponse;
import org.wl.ll.protocol.ContactResponse;
import org.wl.ll.protocol.IconResponse;
import org.wl.ll.protocol.PhoneResponse;
import org.wl.ll.protocol.SMSResponse;
import org.wl.ll.protocol.UnknownResponse;
import org.wl.ll.protocol.VersionResponse;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

/**
 * Created by wiky on 9/16/14.
 * <p/>
 * 处理单个链接的线程
 */
public class ConnectionThread extends Thread {
    /*
     * 请求数据
     */
    private static String REQUEST_PACKAGES = "application";  /* 应用列表 */
    private static String REQUEST_ICON = "icon:";     /* 应用图标 */
    private static String REQUEST_VERSION = "version";    /* 当前手机客户端的版本号 */
    private static String REQUEST_PHONE = "phone";    /* 手机的基本信息 */
    private static String REQUEST_SMS_INBOX = "sms";    /* 短信收件箱 */
    private static String REQUEST_CONTACT = "contact";
    /* 处理一个链接的线程 */
    private Socket socket = null;
    private OutputStream outputStream = null;
    private InputStream inputStream = null;
    private Context mContext = null;

    public ConnectionThread(Context ctx, Socket s) {
        socket = s;
        mContext = ctx;

        MainActivity.LOG(socket.getRemoteSocketAddress().toString() + " is connected!!!");
    }

    private String readCommand(InputStream input){
        try {
            byte[] size = new byte[8];
            int n = input.read(size);
            if(n!=8){
                return null;
            }
            int len=Integer.valueOf(new String(size),16);
            byte[] buf=new byte[len];
            n=input.read(buf);
            if(n!=len){
                return null;
            }
            return new String(buf);
        }catch (Exception e){
            MainActivity.LOG(e.getMessage() != null ? e.getMessage() : "!");
            return null;
        }
    }

    public void run() {
        try {
            inputStream =socket.getInputStream();
            outputStream = socket.getOutputStream();
            String buf;
            while ((buf = readCommand(inputStream)) != null) {
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
                } else if (lower.equals(REQUEST_SMS_INBOX)) {
                    onSMSInboxResponse();
                } else if (lower.equals(REQUEST_CONTACT)) {
                    onContactResponse();
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

    private void onSMSInboxResponse() {
        new SMSResponse(mContext).onResponse(outputStream);
    }

    private void onContactResponse() {
        new ContactResponse(mContext).onResponse(outputStream);
    }

    /*
     * 未知的请求
     * 直接返回该请求数据
     */
    private void onUnknownResponse(String buf) {
        new UnknownResponse(mContext, buf).onResponse(outputStream);
    }
}
