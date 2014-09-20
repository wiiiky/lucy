package org.wl.ama.protocol;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

/**
 * Created by wiky on 9/20/14.
 *
 * 响应的基类
 */
public abstract class Response {

    public abstract String getData();

    public void onResponse(PrintWriter writer){
        writer.write(getData());
        writer.flush();
    }

    public void onResponse(OutputStreamWriter writer) throws IOException {
        writer.write(getData());
        writer.flush();
    }
}
