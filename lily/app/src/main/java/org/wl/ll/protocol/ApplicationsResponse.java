package org.wl.ll.protocol;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import java.util.List;

/**
 * Created by wiky on 9/20/14.
 */
public class ApplicationsResponse extends Response {
    /*
     * sdcard表示安装在SD卡
     * phone表示应用安装在手机内存
     */
    private static String INSTALLED_IN_SDCARD="sdcard";
    private static String INSTALLED_IN_PHONE="phone";

    public ApplicationsResponse(Context ctx) {
        super(ctx);
    }

    /*
     * 返回应用包列表
     * packageName1:appName1:versionName1:installedtime1:installedLocation1:description1\n
     * packageName2:appName2:versionName2:installedtime2:installedLocation2:description2\n
     * \n
     */
    @Override
    public String getString() {
        PackageManager pManager=mContext.getPackageManager();
        List<PackageInfo> packageInfos=pManager.getInstalledPackages(0);
        String data="";
        for (int i=0;i<packageInfos.size();i++){
            PackageInfo info=packageInfos.get(i);
            data=data+getApplicationInfoData(info);
        }
        return getOKAY()+data;
    }

    /*
     * 获取要返回的所有包相关数据
     */
    private String getApplicationInfoData(PackageInfo info){
        String data=getApplicationPackageName(info) +":"+
                getApplicationName(info) +":"+
                getApplicationVersionName(info) + ":" +
                getApplicationInstalledTime(info)+":"+
                getApplicationInstalledLocation(info)+":"+
                getApplicationDescription(info)+
                "\n";
        return  data;
    }

    private String getApplicationDescription(PackageInfo info){
        CharSequence des=info.applicationInfo.loadDescription(mContext.getPackageManager());
        if(des==null){
            return "null";
        }
        return des.toString();
    }

    /*
     * 获取应用包名
     */
    private String getApplicationPackageName(PackageInfo info){
        String name=info.packageName;
        return name;
    }

    /*
     * 获取应用名
     */
    private String getApplicationName(PackageInfo info){
        String name=info.applicationInfo.loadLabel(mContext.getPackageManager()).toString();
        return name;
    }

    private String getApplicationVersionName(PackageInfo info){
        String version=info.versionName;
        return version;
    }

    /*
     * 获取应用的安装位置
     */
    private String getApplicationInstalledLocation(PackageInfo info){
        if ((info.applicationInfo.flags & ApplicationInfo.FLAG_EXTERNAL_STORAGE) !=0){
            return INSTALLED_IN_SDCARD;
        }
        return INSTALLED_IN_PHONE;
    }

    /*
     * 获取应用的安装时间
     */
    private String getApplicationInstalledTime(PackageInfo info){
        String time= Long.toString(info.firstInstallTime);
        return time;
    }
}
