package org.wl.ll.protocol;

import android.content.Context;

import java.io.IOException;
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

    public void onResponse(PrintWriter writer){
        String data=getData();
        writer.write(getLength(data));
        writer.write(data);
        writer.flush();
    }

    public void onResponse(OutputStreamWriter writer) throws IOException {
        String data=getData();
        writer.write(getLength(data));
        writer.write(data);
        writer.flush();
    }

    protected String getLength(String data){
        return String.format("%04x",data.length());
    }
}
