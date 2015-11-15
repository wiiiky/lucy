package org.wiky.lily;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.annotation.Nullable;

import org.wiky.lily.net.Server;

/**
 * Created by wiky on 11/15/15.
 */

public class ServerService extends Service {

    private final IBinder mBinder = new LocalBinder();
    private Server mServer = null;

    @Override
    public void onCreate() {
        mServer = new Server();
        mServer.runInThread();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onDestroy() {
        mServer.stopThread();
        super.onDestroy();
    }

    public class LocalBinder extends Binder {
        ServerService getService() {
            // Return this instance of LocalService so clients can call public methods
            return ServerService.this;
        }
    }

}