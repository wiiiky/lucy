package org.wl.ll.protocol;

import android.content.Context;
import android.util.Log;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by wiky on 9/20/14.
 * 响应的基类
 */
public abstract class Response {

    protected static final int RETCODE_OKAY = 10086;
    protected static final int RETCODE_UNKNOWN_REQUEST = 0;
    protected static final int RETCODE_CONTACT_FAIL = 2;
    protected static final int RETCODE_VERSION_FAIL = 3;
    protected static final int RETCODE_PHONE_FAIL = 4;
    protected static final int RETCODE_APP_FAIL = 5;
    protected static final int RETCODE_SMS_FAIL = 6;

    protected Context mContext;

    public Response(Context ctx) {
        mContext = ctx;
    }

    protected String getString() {
        return null;
    }

    protected byte[] getByte() {
        return null;
    }

    protected ResponseType getType() {
        return ResponseType.RESPONSE_TYPE_STRING;
    }

    /*
     * 因为JSONObject.put会产生异常，所以直接用字符串构造
     */
    protected String error(int code, String s) {
        return "{\"retcode\":" + code + ",\"result\":\"" + s + "\"}";
    }

    public void onResponse(OutputStream writer) {
        byte[] data = null;
        if (getType() == ResponseType.RESPONSE_TYPE_BYTE) {
            data = getByte();
        } else {
            data = getString().getBytes();
        }
        try {
            writer.write(getLength(data), 0, 8);
            writer.write(data, 0, data.length);
            writer.flush();
        } catch (IOException e) {
            Log.e("IOException while writing", e.getMessage());
        }
    }

    protected byte[] getLength(byte[] data) {
        return String.format("%08x", data.length).getBytes();
    }

    protected String permissionDenied() {
        return "Permission Denied";
    }

    protected enum ResponseType {
        RESPONSE_TYPE_STRING,
        RESPONSE_TYPE_BYTE,
    }
}
