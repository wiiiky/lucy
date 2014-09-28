package org.wl.ll.protocol;

import android.app.ActivityManager;
import android.content.Context;
import android.os.StatFs;
import android.telephony.TelephonyManager;
import android.text.format.Formatter;

import org.wl.ll.MainActivity;

/**
 * Created by wiky on 9/27/14.
 *
 * 返回基本的手机信息
 *
 * 手机型号，手机品牌，手机号码（如果有）,可用内存大小，总内存大小，sdcard可用大小，sdcard总大小,
 * 手机内部存储器可用容量，手机内部存储器总容量
 */
public class PhoneResponse extends Response {
    public PhoneResponse(Context ctx) {
        super(ctx);
    }

    @Override
    public String getData() {
        String data=getPhoneInfo()+ getMemorySize()+getExternalStorageSize()+ getDataStorageSize();
        return getOKAY()+data;
    }

    private String getPhoneInfo(){
        String mtype="unknown";
        String mtyb="unknown";
        String number="0";
        try {
            TelephonyManager mTm = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
            mtype = android.os.Build.MODEL; // 手机型号
            mtyb = android.os.Build.BRAND;//手机品牌
            number = mTm.getLine1Number(); // 手机号码，有的可得，有的不可得
        }catch (Exception e){
            MainActivity.LOG(e.getMessage());
        }
        return mtype+"\n"+mtyb+"\n"+number+"\n";
    }

    private String getMemorySize() {// 获取android当前可用内存大小

        ActivityManager am = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        am.getMemoryInfo(mi);

        return Formatter.formatFileSize(mContext, mi.availMem)+"\n"+
                Formatter.formatFileSize(mContext, mi.totalMem)+"\n";// 将获取的内存大小规格化
    }

    private String getExternalStorageSize(){
        String path=getExternalStoragePath();
        long total=0;
        long avail=0;
        try {
            StatFs fstat = new StatFs(path);
            int size=fstat.getBlockSize();
            total=size*fstat.getBlockCount();
            avail = size*fstat.getAvailableBlocks();
        }catch (Exception e){
        }

        return Formatter.formatFileSize(mContext,avail)+"\n"+
                Formatter.formatFileSize(mContext,total)+"\n";
    }

    /*
     * 获取手机内部存储器大小
     */
    private String getDataStorageSize(){
        long total=0;
        long avail=0;
        try{
            StatFs statFs=new StatFs(android.os.Environment.getDataDirectory().getPath());
            long size=statFs.getBlockSizeLong();
            total=size*statFs.getBlockCountLong();
            avail=size*statFs.getAvailableBlocksLong();
        }catch (Exception e){

        }
        return Formatter.formatFileSize(mContext,avail)+"\n"+
                Formatter.formatFileSize(mContext,total)+"\n";
    }

    private String getExternalStoragePath() {
        // 获取SdCard状态
        String state = android.os.Environment.getExternalStorageState();
        // 判断SdCard是否存在并且是可用的
        if (android.os.Environment.MEDIA_MOUNTED.equals(state)) {
            if (android.os.Environment.getExternalStorageDirectory().canWrite()) {
                return android.os.Environment.getExternalStorageDirectory().getPath();
            }
        }
        return null;
    }
}
