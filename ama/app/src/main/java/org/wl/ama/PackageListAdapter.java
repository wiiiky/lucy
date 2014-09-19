package org.wl.ama;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by wiky on 9/19/14.
 */
public class PackageListAdapter extends BaseAdapter {
    private ArrayList<PackageListModel> data;
    private Context mContext;

    public PackageListAdapter(Context context,ArrayList<PackageListModel> list){
        data=list;
        mContext=context;
    }

    @Override
    public int getCount() {
        return data.size();
    }

    @Override
    public Object getItem(int i) {
        return data.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        ViewHolder holder=getViewHolder(view);
        PackageListModel d=(PackageListModel)getItem(i);
        holder.icon.setImageDrawable(d.icon);
        holder.packageName.setText(d.packageName);
        holder.appName.setText(d.appName);
        return holder.view;
    }

    private final class ViewHolder{
        public View view;
        public ImageView icon;
        public TextView packageName;
        public TextView appName;
    }

    private ViewHolder getViewHolder(View v){
        ViewHolder holder=null;
        if(v==null){
            holder=new ViewHolder();
            holder.view= LayoutInflater.from(mContext).inflate(R.layout.listview_packages_item,null);
            holder.icon=(ImageView)holder.view.findViewById(R.id.imgIcon);
            holder.packageName=(TextView)holder.view.findViewById(R.id.tvPackageName);
            holder.appName=(TextView)holder.view.findViewById(R.id.tvAppName);
            holder.view.setTag(holder);
        }else{
            holder=(ViewHolder)v.getTag();
        }
        return holder;
    }
}
