package org.wiky.lily;

import android.app.Application;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

/**
 * Created by wiky on 11/15/15.
 */
public class LilyApplication extends Application {

    public static AdbConfig mAdbConfig = new AdbConfig();
    public static LilyApplication mInstance = null;
    private ServerService mService;
    private boolean mBound;
    /**
     * Defines callbacks for service binding, passed to bindService()
     */
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            // We've bound to LocalService, cast the IBinder and get LocalService instance
            ServerService.LocalBinder binder = (ServerService.LocalBinder) service;
            mService = binder.getService();
            mBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            mBound = false;
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();

        mInstance = this;

        Intent intent = new Intent(this, ServerService.class);
        bindService(intent, mConnection, BIND_AUTO_CREATE);
    }

    @Override
    public void onTerminate() {
        unbindService(mConnection);
        super.onTerminate();
    }

    public static class AdbConfig {
        public int port = 42132;
        public String address = "127.0.0.1";
    }


}
