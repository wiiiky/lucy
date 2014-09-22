package org.wl.ama.model;

import android.graphics.drawable.Drawable;

/**
 * Created by wiky on 9/19/14.
 */
public class ApplicationListModel {
    public Drawable icon;
    public String packageName;
    public String appName;

    public ApplicationListModel(Drawable icon, String packageName, String appName){
        this.icon=icon;
        this.packageName=packageName;
        this.appName=appName;
    }
}
