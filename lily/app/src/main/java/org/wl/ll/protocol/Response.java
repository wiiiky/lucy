package org.wl.ll.protocol;

import android.content.Context;

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
    private static String OKAY = "OKAY";
    private static String FAIL = "FAIL";
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

    protected String getOKAY() {
        return OKAY;
    }

    protected String getFAIL() {
        return FAIL;
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
            writer.write(getLength(data), 0, 4);
            writer.write(data, 0, data.length);
            writer.flush();
        } catch (IOException e) {
        }
    }

    protected byte[] getLength(byte[] data) {
        return String.format("%04x", data.length).getBytes();
    }

    protected String getLength(String data) {
        return String.format("%04x", data.getBytes().length);
    }

    protected byte[] mergeBytes(byte[] b1, byte[] b2) {
        byte[] b3 = new byte[b1.length + b2.length];
        System.arraycopy(b1, 0, b3, 0, b1.length);
        System.arraycopy(b2, 0, b3, b1.length, b2.length);
        return b3;
    }

    protected String permissionDenied() {
        return "Permission Denied";
    }

    protected enum ResponseType {
        RESPONSE_TYPE_STRING,
        RESPONSE_TYPE_BYTE,
    }
}
