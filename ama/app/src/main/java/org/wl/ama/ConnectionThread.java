package org.wl.ama;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.util.Log;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;

/**
 * Created by wiky on 9/16/14.
 *
 * 处理单个链接的线程
 */
public class ConnectionThread extends Thread {
    /* 处理一个链接的线程，目前只是echo */
    private Socket socket=null;
    private PrintWriter printWriter=null;
    private BufferedReader bufferedReader=null;
    private Context mContext=null;

    public ConnectionThread(Context ctx,Socket s){
        socket=s;
        mContext=ctx;
    }

    public void run(){
        try {
            InputStreamReader inputReader=new InputStreamReader(socket.getInputStream());
            printWriter=new PrintWriter(socket.getOutputStream());
            bufferedReader=new BufferedReader(inputReader);
            String buf;
            while((buf=bufferedReader.readLine())!=null){
                String lower=buf.toLowerCase();
                if (lower.equals("packages")){
                    onPackagesResponse();
                }else {
                    onUnknownResponse(buf);
                }
            }
        } catch (IOException e) {
            Log.e("IOException", e.getMessage() != null ? e.getMessage() : "!");
        }finally {
            cleanup();
        }
    }

    private void cleanup(){
        if (printWriter != null) {
            printWriter.close();
        }
        try{
            if(bufferedReader!=null){
                bufferedReader.close();
            }
        }catch (IOException e){
        }
        try {
            socket.close();
        } catch (IOException e) {
        }
    }

    private void onPackagesResponse(){
        PackageManager pManager=mContext.getPackageManager();
        List<PackageInfo> packageInfos=pManager.getInstalledPackages(0);
        for (int i=0;i<packageInfos.size();i++){
            PackageInfo info=packageInfos.get(i);
            String data=info.packageName+":" +
                    info.applicationInfo.loadLabel(pManager).toString() +
                    info.versionName + ":" +
                    "\n";
            printWriter.write(data);
        }
        printWriter.write("\n");
        printWriter.flush();
    }

    private void onUnknownResponse(String buf){
        printWriter.write(buf);
        printWriter.flush();
    }
}
