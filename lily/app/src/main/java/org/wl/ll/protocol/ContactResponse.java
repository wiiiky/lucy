package org.wl.ll.protocol;

import android.content.Context;
import android.database.Cursor;
import android.provider.ContactsContract;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.wl.ll.model.ContactModel;

import java.util.ArrayList;

/**
 * Created by wiky on 11/2/14.
 * 联系人列表
 */
public class ContactResponse extends Response {
    public ContactResponse(Context ctx) {
        super(ctx);
    }

    @Override
    protected String getString() {
        ArrayList<ContactModel> list = getContactList();
        if (list == null) {
            return error(RETCODE_CONTACT_FAIL, permissionDenied());
        }
        JSONObject root = new JSONObject();
        try {
            JSONArray array = new JSONArray();
            root.put("retcode", RETCODE_OKAY);
            for (int i = 0; i < list.size(); i++) {
                JSONObject obj = new JSONObject();
                ContactModel model = list.get(i);
                obj.put("id", new Integer(model.id));
                obj.put("name", model.name);
                obj.put("number", model.number);
                array.put(obj);
            }
            root.put("result", array);
        } catch (JSONException e) {
            return error(RETCODE_CONTACT_FAIL, e.getMessage());
        }
        return root.toString();
    }

    private ArrayList<ContactModel> getContactList() {
        ArrayList<ContactModel> list = null;

        Cursor cursor = mContext.getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,
                null, null, null, ContactsContract.Contacts._ID + " asc");
        if (cursor.moveToFirst()) {
            list = new ArrayList<ContactModel>();
            int idIndex = cursor.getColumnIndex(ContactsContract.Contacts._ID);
            int nameIndex = cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
            int countIndex = cursor.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER);
            do {
                String id = cursor.getString(idIndex);
                String name = cursor.getString(nameIndex);
                int count = cursor.getInt(countIndex);
                ArrayList<String> numbers = new ArrayList<String>();
                if (count > 0) {
                    Cursor phones = mContext.getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
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
