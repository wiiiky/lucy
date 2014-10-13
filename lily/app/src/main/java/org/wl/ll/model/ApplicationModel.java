package org.wl.ll.model;

import android.graphics.drawable.Drawable;

/**
 * Created by wiky on 9/19/14.
 */
public class ApplicationModel {
    public Drawable icon;
    public String packageName;
    public String appName;

    public ApplicationModel(Drawable icon, String packageName, String appName) {
        this.icon = icon;
        this.packageName = packageName;
        this.appName = appName;
    }
}
