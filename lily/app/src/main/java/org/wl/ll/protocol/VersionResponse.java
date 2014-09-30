package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

/**
 * Created by wiky on 9/22/14.
 */
public class VersionResponse extends Response {


    public VersionResponse(Context ctx) {
        super(ctx);
    }

    @Override
    public String getString() {
        PackageInfo info;
        try {
            info=mContext.getPackageManager().getPackageInfo(mContext.getPackageName(),0);
        } catch (PackageManager.NameNotFoundException e) {
            return "unknown";
        }
        return getOKAY()+Integer.toString(info.versionCode);
    }

    @Override
    public ResponseType getType() {
        return ResponseType.RESPONSE_TYPE_STRING;
    }
}
