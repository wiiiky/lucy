package org.wl.ll.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import org.wl.ll.R;
import org.wl.ll.model.ContactModel;

import java.util.ArrayList;

/**
 * Created by wiky on 10/29/14.
 */
public class ContactAdapter extends BaseAdapter{
    protected Context context;
    protected ArrayList<ContactModel> data;

    public ContactAdapter(Context ctx,ArrayList<ContactModel> list){
        context=ctx;
        data=list;
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
        ContactModel contact=(ContactModel)getItem(i);
        holder.tvId.setText(contact.id);
        holder.tvName.setText(contact.name);
        if(holder.tvNumber.length()>0) {
            holder.tvNumber.setText(contact.number.get(0));
        }else{
            holder.tvNumber.setText("NULL");
        }
        return holder.view;
    }

    private final class ViewHolder{
        View view;
        TextView tvId;
        TextView tvName;
        TextView tvNumber;
    }

    private ViewHolder getViewHolder(View v){
        ViewHolder holder;
        if(v==null){
            holder = new ViewHolder();
            v = LayoutInflater.from(context).inflate(R.layout.contact_item, null);
            holder.tvId =(TextView) v.findViewById(R.id.tvId);
            holder.tvName = (TextView)v.findViewById(R.id.tvName);
            holder.tvNumber = (TextView) v.findViewById(R.id.tvNumber);
            holder.view=v;
            v.setTag(holder);
        }else{
            holder=(ViewHolder)v.getTag();
        }
        return holder;
    }
}
