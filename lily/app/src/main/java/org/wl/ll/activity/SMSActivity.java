package org.wl.ll.activity;

import android.app.Activity;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import org.wl.ll.R;
import org.wl.ll.adapter.SMSAdapter;
import org.wl.ll.model.SMSModel;

import java.net.URI;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class SMSActivity extends Activity {

    private ListView listView = null;

    private final String URI_SMS_INBOX = "content://sms/inbox";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sms);

        listView = (ListView) findViewById(R.id.list);

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                ArrayList<SMSModel> list = readSMS();
                if(list==null){
                    return;
                }
                SMSAdapter adapter = new SMSAdapter(SMSActivity.this, list);
                listView.setAdapter(adapter);
            }
        }, 100);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.sm, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public void onBackPressed() {
        finish();
        overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
    }

    private final String SMS_COLUMN_ID = "_id";
    private final String SMS_COLUMN_ADDRESS = "address";
    private final String SMS_COLUMN_PERSON = "person";
    private final String SMS_COLUMN_BODY = "body";
    private final String SMS_COLUMN_DATE = "date";
    private final String SMS_COLUMN_TYPE = "type";

    private ArrayList<SMSModel> readSMS() {
        ArrayList<SMSModel> list = new ArrayList<SMSModel>();
        try {
            Uri uri = Uri.parse(URI_SMS_INBOX);
            String[] projection = new String[]{SMS_COLUMN_ID, SMS_COLUMN_ADDRESS,
                    SMS_COLUMN_PERSON, SMS_COLUMN_BODY,
                    SMS_COLUMN_DATE, SMS_COLUMN_TYPE};
            Cursor cursor = getContentResolver().query(uri, projection, null, null, "date desc");
            if (cursor.moveToFirst()) {
                int idIndex = cursor.getColumnIndex(SMS_COLUMN_ID);
                int addressIndex = cursor.getColumnIndex(SMS_COLUMN_ADDRESS);
                int personIndex = cursor.getColumnIndex(SMS_COLUMN_PERSON);
                int bodyIndex = cursor.getColumnIndex(SMS_COLUMN_BODY);
                int dateIndex = cursor.getColumnIndex(SMS_COLUMN_DATE);
                int typeIndex = cursor.getColumnIndex(SMS_COLUMN_TYPE);

                do {
                    String body = cursor.getString(bodyIndex);
                    int person = cursor.getInt(personIndex);
                    String address = cursor.getString(addressIndex);
                    long date = cursor.getLong(dateIndex);

                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
                    Date d = new Date(date);
                    String dateStr = dateFormat.format(d);

                    SMSModel sms = new SMSModel(body, dateStr, address);
                    list.add(sms);
                } while (cursor.moveToNext());
            }
        }catch (Exception e){
            return null;
        }

        return list;
    }
}
