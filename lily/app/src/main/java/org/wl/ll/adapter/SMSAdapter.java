package org.wl.ll.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import org.wl.ll.R;
import org.wl.ll.model.SMSModel;

import java.util.ArrayList;

/**
 * Created by wiky on 10/13/14.
 */
public class SMSAdapter extends BaseAdapter {
    private ArrayList<SMSModel> data = null;
    private Context context;

    public SMSAdapter(Context _context, ArrayList<SMSModel> list) {
        data = list;
        context = _context;
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
        SMSModel sms = (SMSModel) getItem(i);
        holder.tvContent.setText(sms.content);
        holder.tvTime.setText(sms.time);
        holder.tvSender.setText(sms.sender);
        return holder.view;
    }

    private final class ViewHolder {
        public View view;
        public TextView tvContent;
        public TextView tvTime;
        public TextView tvSender;
    }

    private ViewHolder getViewHolder(View v) {
        ViewHolder holder;
        if (v == null) {
            holder = new ViewHolder();
            holder.view = LayoutInflater.from(context).inflate(R.layout.sms_item, null);
            holder.tvContent = (TextView) holder.view.findViewById(R.id.tvContent);
            holder.tvTime = (TextView) holder.view.findViewById(R.id.tvTime);
            holder.tvSender = (TextView) holder.view.findViewById(R.id.tvSender);
            holder.view.setTag(holder);
        } else {
            holder = (ViewHolder) v.getTag();
        }
        return holder;
    }
}
