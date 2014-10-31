package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageStats;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.List;

/**
 * Created by wiky on 9/20/14.
 */
public class ApplicationsResponse extends Response {
    /*
     * sdcard表示安装在SD卡
     * phone表示应用安装在手机内存
     */
    private static String INSTALLED_IN_SDCARD = "sdcard";
    private static String INSTALLED_IN_PHONE = "phone";

    public ApplicationsResponse(Context ctx) {
        super(ctx);
    }

    @Override
    public String getString() {
        PackageManager pManager = mContext.getPackageManager();
        List<PackageInfo> packageInfos = pManager.getInstalledPackages(0);
        JSONObject root = new JSONObject();
        try {
            root.put("retcode", RETCODE_OKAY);
            JSONArray array = new JSONArray();
            for (int i = 0; i < packageInfos.size(); i++) {
                PackageInfo info = packageInfos.get(i);
                JSONObject obj = new JSONObject();
                obj.put("package_name", getApplicationPackageName(info));
                obj.put("app_name", getApplicationName(info));
                obj.put("version_name", getApplicationVersionName(info));
                obj.put("location", getApplicationInstalledLocation(info));
                obj.put("install_time", getApplicationInstalledTime(info));
                obj.put("type", getApplicationType(info));
                obj.put("size", getApplicationSize(info));
                obj.put("description", getApplicationDescription(info));
                array.put(obj);
            }
            root.put("result", array);
        } catch (Exception e) {
            error(RETCODE_APP_FAIL, e.getMessage());
        }
        return root.toString();
    }

    private String getApplicationDescription(PackageInfo info) {
        CharSequence des = info.applicationInfo.loadDescription(mContext.getPackageManager());
        if (des == null) {
            return "null";
        }
        return des.toString();
    }

    /*
     * 获取应用包名
     */
    private String getApplicationPackageName(PackageInfo info) {
        String name = info.packageName;
        return name;
    }

    /*
     * 获取应用名
     */
    private String getApplicationName(PackageInfo info) {
        String name = info.applicationInfo.loadLabel(mContext.getPackageManager()).toString();
        return name;
    }

    private String getApplicationVersionName(PackageInfo info) {
        String version = info.versionName;
        return version;
    }

    /*
     * 获取应用的安装位置
     */
    private String getApplicationInstalledLocation(PackageInfo info) {
        if ((info.applicationInfo.flags & ApplicationInfo.FLAG_EXTERNAL_STORAGE) != 0) {
            return INSTALLED_IN_SDCARD;
        }
        return INSTALLED_IN_PHONE;
    }

    /*
     * 获取应用的安装时间
     */
    private String getApplicationInstalledTime(PackageInfo info) {
        String time = Long.toString(info.firstInstallTime);
        return time;
    }

    private String getApplicationType(PackageInfo info) {
        if ((info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) > 0) {
            return "system";    /* 系统应用 */
        }
        return "user";
    }

    private long getApplicationSize(PackageInfo info) {
        /* not work, FIXME */
        PackageStats packageStats = new PackageStats(info.packageName);
        long size = packageStats.cacheSize + packageStats.codeSize + packageStats.dataSize +
                packageStats.externalCacheSize + packageStats.externalDataSize + packageStats.externalCodeSize;
        return size;
    }
}
