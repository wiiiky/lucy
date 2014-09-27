package org.wl.ll.protocol;

import android.content.Context;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

/**
 * Created by wiky on 9/20/14.
 *
 * 响应的基类
 */
public abstract class Response {

    protected Context mContext;

    public Response(Context ctx){
        mContext=ctx;
    }

    public abstract String getData();

    protected String getOKAY(){
        return "OKAY";
    }

    protected String getFAIL(){
        return "FAIL";
    }

    public void onResponse(OutputStream writer){
        byte[] data=getData().getBytes();
        try {
            writer.write(getLength(data),0,4);
            writer.write(data,0,data.length);
            writer.flush();
        } catch (IOException e) {
        }
    }

    protected byte[] getLength(byte[] data){
        return String.format("%04x",data.length).getBytes();
    }
}
