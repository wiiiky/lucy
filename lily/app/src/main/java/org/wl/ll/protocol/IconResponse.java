package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

import org.wl.ll.R;

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
        PackageInfo info;
        Drawable drawable;
        try {
            info = pManager.getPackageInfo(packageName, 0);
            drawable = info.applicationInfo.loadIcon(pManager);
        } catch (PackageManager.NameNotFoundException e) {
            drawable = mContext.getResources().getDrawable(R.drawable.ic_launcher);
        }
        Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream);

        byte[] icon = byteArrayOutputStream.toByteArray();
        return icon;
    }

    @Override
    public Response.ResponseType getType() {
        return ResponseType.RESPONSE_TYPE_BYTE;
    }

}
