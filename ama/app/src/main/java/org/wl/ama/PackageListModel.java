package org.wl.ama;

import android.graphics.drawable.Drawable;

/**
 * Created by wiky on 9/19/14.
 */
public class PackageListModel {
    public Drawable icon;
    public String packageName;
    public String appName;

    public PackageListModel(Drawable icon,String packageName,String appName){
        this.icon=icon;
        this.packageName=packageName;
        this.appName=appName;
    }
}
