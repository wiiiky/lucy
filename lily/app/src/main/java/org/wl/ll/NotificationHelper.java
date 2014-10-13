package org.wl.ll;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;

/**
 * Created by wiky on 9/16/14.
 */
public class NotificationHelper {

    public static void show(String title, String text, Activity ac) {
        Intent intent = new Intent(ac, ac.getClass());
        PendingIntent pIntent = PendingIntent.getActivity(ac, 0, intent, 0);

        // build notification
        Notification n = new Notification.Builder(ac)
                .setContentTitle(title)
                .setContentText(text)
                .setSmallIcon(R.drawable.ic_launcher)
                .setContentIntent(pIntent).build();

        n.flags = Notification.FLAG_AUTO_CANCEL;
        NotificationManager notificationManager =
                (NotificationManager) ac.getSystemService(ac.NOTIFICATION_SERVICE);
        notificationManager.notify(0, n);
    }

    public static void cancelAll(Activity ac) {
        NotificationManager notificationManager =
                (NotificationManager) ac.getSystemService(ac.NOTIFICATION_SERVICE);
        notificationManager.cancelAll();
    }
}
