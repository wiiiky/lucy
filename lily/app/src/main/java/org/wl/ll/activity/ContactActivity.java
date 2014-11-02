package org.wl.ll.activity;

import android.app.Activity;
import android.database.Cursor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import org.wl.ll.R;
import org.wl.ll.adapter.ContactAdapter;
import org.wl.ll.model.ContactModel;

import java.util.ArrayList;

public class ContactActivity extends Activity {

    private ListView lv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contact);

        lv = (ListView) findViewById(R.id.lv);

        new ReadContactTask().execute();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.contact, menu);
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
        moveTaskToBack(true);
        overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
    }

    private class ReadContactTask extends AsyncTask<Void, Void, ArrayList<ContactModel>> {

        @Override
        protected ArrayList<ContactModel> doInBackground(Void... voids) {
            return getContactList();
        }

        @Override
        public void onPostExecute(ArrayList<ContactModel> list) {
            lv.setAdapter(new ContactAdapter(ContactActivity.this, list));
        }

        private ArrayList<ContactModel> getContactList() {
            ArrayList<ContactModel> list = new ArrayList<ContactModel>();

            Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,
                    null, null, null, ContactsContract.Contacts._ID + " asc");
            if (cursor.moveToFirst()) {
                int idIndex = cursor.getColumnIndex(ContactsContract.Contacts._ID);
                int nameIndex = cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
                int countIndex = cursor.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER);
                do {
                    String id = cursor.getString(idIndex);
                    String name = cursor.getString(nameIndex);
                    int count = cursor.getInt(countIndex);
                    ArrayList<String> numbers = new ArrayList<String>();
                    if (count > 0) {
                        Cursor phones = getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                                null, ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=" + id, null, null);
                        if (phones.moveToFirst()) {
                            int numberIndex = phones.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER);
                            do {
                                String number = phones.getString(numberIndex);
                                numbers.add(number);
                            } while (phones.moveToNext());
                        }
                        phones.close();
                    }
                    ContactModel model = new ContactModel(id, name, numbers);
                    list.add(model);
                } while (cursor.moveToNext());
            }
            cursor.close();

            return list;
        }
    }
}
