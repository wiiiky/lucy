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
        writer.write(getData());
        writer.flush();
    }

    public void onResponse(OutputStreamWriter writer) throws IOException {
        writer.write(getData()+"\n");
        writer.flush();
    }
}
