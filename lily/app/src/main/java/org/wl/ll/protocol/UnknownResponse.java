package org.wl.ll.protocol;

import android.content.Context;

/**
 * Created by wiky on 9/23/14.
 */
public class UnknownResponse extends Response {
    private String data;

    public UnknownResponse(Context ctx, String buf) {
        super(ctx);
        data = buf;
    }

    @Override
    public String getString() {
        return error(RETCODE_UNKNOWN_REQUEST, data);
    }

}
