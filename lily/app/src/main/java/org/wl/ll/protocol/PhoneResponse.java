package org.wl.ll.protocol;

import android.app.ActivityManager;
import android.content.Context;
import android.os.StatFs;
import android.telephony.TelephonyManager;

import org.json.JSONObject;
import org.wl.ll.MainActivity;

import java.io.BufferedReader;
import java.io.FileReader;

/**
 * Created by wiky on 9/27/14.
 * <p/>
 * 返回基本的手机信息
 * <p/>
 * 手机型号，手机品牌，手机号码（如果有）,可用内存大小，总内存大小，sdcard可用大小，sdcard总大小,
 * 手机内部存储器可用容量，手机内部存储器总容量
 */
public class PhoneResponse extends Response {
    public PhoneResponse(Context ctx) {
        super(ctx);
    }

    @Override
    public String getString() {
        JSONObject root = new JSONObject();
        try {
            root.put("retcode", RETCODE_OKAY);
            /* 获取手机型号信息 */
            String mtype = "unknown";
            String mtyb = "unknown";
            String number = "0";
            try {
                TelephonyManager mTm = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
                mtype = android.os.Build.MODEL; // 手机型号
                mtyb = android.os.Build.BRAND;//手机品牌
                number = mTm.getLine1Number(); // 手机号码，有的可得，有的不可得
            } catch (Exception e) {
                MainActivity.LOG(e.getMessage());
            }
            root.put("model", mtype);
            root.put("brand", mtyb);
            root.put("number", number);
            /* 获取内存信息 */
            ActivityManager am = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
            ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
            am.getMemoryInfo(mi);
            root.put("avail_mem", mi.availMem);
            root.put("total_mem", getTotalMemory());
            /* 存储空间信息 */
            String path = getExternalStoragePath();
            long total = 0;
            long avail = 0;
            try {
                StatFs fstat = new StatFs(path);
                int size = fstat.getBlockSize();
                total = size * fstat.getBlockCount();
                avail = size * fstat.getAvailableBlocks();
            } catch (Exception e) {
            }
            root.put("avail_external", avail);
            root.put("total_external", total);
            /* 手机内部存储空间 */
            total = 0;
            avail = 0;
            try {
                StatFs statFs = new StatFs(android.os.Environment.getDataDirectory().getPath());
                long size = statFs.getBlockSize();
                total = size * statFs.getBlockCount();
                avail = size * statFs.getAvailableBlocks();
            } catch (Exception e) {
            }
            root.put("avail_data", avail);
            root.put("total_data", total);
        } catch (Exception e) {
            return error(RETCODE_PHONE_FAIL, e.getMessage());
        }
        return root.toString();
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

    private long getTotalMemory() {
        String infoFile = "/proc/meminfo";
        String line;
        String[] arrayOfString;
        long memory = 0;
        try {
            FileReader reader = new FileReader(infoFile);
            BufferedReader bufferedReader = new BufferedReader(reader, 4096 * 2);
            line = bufferedReader.readLine();
            arrayOfString = line.split("\\s+");
            memory = Long.valueOf(arrayOfString[1]).longValue() * 1024; //KB=>B
            bufferedReader.close();
            reader.close();
        } catch (Exception e) {

        }
        return memory;
    }
}
