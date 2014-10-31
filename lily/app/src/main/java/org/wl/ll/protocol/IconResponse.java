package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

import java.io.ByteArrayOutputStream;

/**
 * Created by wiky on 9/30/14.
 * <p/>
 * 返回应用图标
 */
public class IconResponse extends Response {
    private String packageName;

    public IconResponse(Context ctx, String name) {
        super(ctx);
        packageName = name;
    }

    @Override
    public byte[] getByte() {
        PackageManager pManager = mContext.getPackageManager();
        PackageInfo info = null;
        try {
            info = pManager.getPackageInfo(packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            return getFAIL().getBytes();
        }
        Drawable drawable = info.applicationInfo.loadIcon(pManager);
        Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream);

        byte[] okay = getOKAY().getBytes();
        byte[] icon = byteArrayOutputStream.toByteArray();
        return mergeBytes(okay, icon);
    }

    @Override
    public Response.ResponseType getType() {
        return ResponseType.RESPONSE_TYPE_BYTE;
    }

}
