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
    public String getData() {
        PackageInfo info;
        try {
            info=mContext.getPackageManager().getPackageInfo(mContext.getPackageName(),0);
        } catch (PackageManager.NameNotFoundException e) {
            return "unknown\n";
        }
        return "OKAY"+Integer.toString(info.versionCode);
    }
}
