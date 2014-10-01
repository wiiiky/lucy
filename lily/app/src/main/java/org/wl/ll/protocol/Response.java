package org.wl.ll.protocol;

import android.content.Context;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by wiky on 9/20/14.
 *
 * 响应的基类
 */
public abstract class Response {

    protected Context mContext;

    private static String OKAY="OKAY";
    private static String FAIL="FAIL";

    public Response(Context ctx){
        mContext=ctx;
    }

    protected String getString(){return null;};
    protected byte[] getByte(){return null;}
    protected ResponseType getType(){
        return ResponseType.RESPONSE_TYPE_STRING;
    }

    protected String getOKAY(){
        return OKAY;
    }

    protected String getFAIL(){
        return FAIL;
    }

    public void onResponse(OutputStream writer){
        byte[] data=null;
        if(getType()==ResponseType.RESPONSE_TYPE_BYTE){
            data=getByte();
        }else{
            data=getString().getBytes();
        }
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

    protected enum ResponseType{
        RESPONSE_TYPE_STRING,
        RESPONSE_TYPE_BYTE,
    }

    protected byte[] mergeBytes(byte[] b1,byte[] b2){
        byte[] b3=new byte[b1.length+b2.length];
        System.arraycopy(b1,0,b3,0,b1.length);
        System.arraycopy(b2,0,b3,b1.length,b2.length);
        return b3;
    }
}
