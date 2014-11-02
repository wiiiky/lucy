package org.wl.ll.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import org.wl.ll.R;
import org.wl.ll.model.ApplicationModel;

import java.util.ArrayList;

/**
 * Created by wiky on 9/19/14.
 */
public class ApplicationAdapter extends BaseAdapter {
    private ArrayList<ApplicationModel> data;
    private Context mContext;

    public ApplicationAdapter(Context context, ArrayList<ApplicationModel> list) {
        data = list;
        mContext = context;
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
        ViewHolder holder = getViewHolder(view);
        ApplicationModel d = (ApplicationModel) getItem(i);
        holder.imgIcon.setImageDrawable(d.icon);
        holder.tvPackageName.setText(d.packageName);
        holder.tvAppName.setText(d.appName);
        return holder.view;
    }

    private ViewHolder getViewHolder(View v) {
        ViewHolder holder = null;
        if (v == null) {
            holder = new ViewHolder();
            holder.view = LayoutInflater.from(mContext).inflate(R.layout.application_item, null);
            holder.imgIcon = (ImageView) holder.view.findViewById(R.id.imgIcon);
            holder.tvPackageName = (TextView) holder.view.findViewById(R.id.tvPackageName);
            holder.tvAppName = (TextView) holder.view.findViewById(R.id.tvAppName);
            holder.view.setTag(holder);
        } else {
            holder = (ViewHolder) v.getTag();
        }
        return holder;
    }

    private final class ViewHolder {
        public View view;
        public ImageView imgIcon;
        public TextView tvPackageName;
        public TextView tvAppName;
    }
}
