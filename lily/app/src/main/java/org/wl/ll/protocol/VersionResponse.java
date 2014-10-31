package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.PackageInfo;

import org.json.JSONObject;

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
        JSONObject root = new JSONObject();
        try {
            info = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0);
            root.put("retcode", RETCODE_OKAY);
            root.put("result", Integer.toString(info.versionCode));
        } catch (Exception e) {
            return error(RETCODE_VERSION_FAIL, e.getMessage());
        }
        return root.toString();
    }
}
