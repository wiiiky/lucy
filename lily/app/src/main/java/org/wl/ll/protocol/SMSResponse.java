package org.wl.ll.protocol;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Created by wiky on 10/14/14.
 * 短信收件箱内容
 * date:address:person:00x4body
 * <p/>
 * date是时间
 * address是电话号码
 * person是电话号码在联系人列表中对应的项
 * 00x4body是以四字节16进制长度开头的短信内容
 */
public class SMSResponse extends Response {
    private final String URI_SMS_INBOX = "content://sms/inbox";
    private final String URI_SMS_ALL = "content://sms/";

    private final String SMS_COLUMN_ID = "_id";
    private final String SMS_COLUMN_ADDRESS = "address";
    private final String SMS_COLUMN_PERSON = "person";
    private final String SMS_COLUMN_BODY = "body";
    private final String SMS_COLUMN_DATE = "date";
    private final String SMS_COLUMN_TYPE = "type";
    private final String SMS_COLUMN_THREAD = "thread_id";

    public SMSResponse(Context ctx) {
        super(ctx);
    }

    @Override
    protected String getString() {
        JSONObject root = new JSONObject();
        try {
            root.put("retcode", RETCODE_OKAY);
            Uri uri = Uri.parse(URI_SMS_ALL);
            String[] projection = new String[]{SMS_COLUMN_ID, SMS_COLUMN_ADDRESS,
                    SMS_COLUMN_PERSON, SMS_COLUMN_BODY,
                    SMS_COLUMN_DATE, SMS_COLUMN_TYPE, SMS_COLUMN_THREAD};
            Cursor cursor = mContext.getContentResolver().query(uri, projection, null, null, "date desc");
            if (cursor.moveToFirst()) {
                JSONArray array = new JSONArray();
                int idIndex = cursor.getColumnIndex(SMS_COLUMN_ID);
                int addressIndex = cursor.getColumnIndex(SMS_COLUMN_ADDRESS);
                int personIndex = cursor.getColumnIndex(SMS_COLUMN_PERSON);
                int bodyIndex = cursor.getColumnIndex(SMS_COLUMN_BODY);
                int dateIndex = cursor.getColumnIndex(SMS_COLUMN_DATE);
                int typeIndex = cursor.getColumnIndex(SMS_COLUMN_TYPE);
                int threadIndex = cursor.getColumnIndex(SMS_COLUMN_THREAD);

                do {
                    int type = cursor.getInt(typeIndex);

                    if (type == 1 || type == 2) { /* 1是接受2是发送 */
                        String body = cursor.getString(bodyIndex);
                        int person = cursor.getInt(personIndex);
                        String address = cursor.getString(addressIndex);
                        long date = cursor.getLong(dateIndex);
                        int threadId = cursor.getInt(threadIndex);

                        JSONObject object = new JSONObject();
                        object.put("thread_id", threadId);
                        object.put("type", type);
                        object.put("date", date / 1000);
                        object.put("address", address);
                        object.put("person", person);
                        object.put("body", body);

                        array.put(object);
                    }
                } while (cursor.moveToNext());
                root.put("result", array);
            } else {  //没有权限
                return error(RETCODE_SMS_FAIL, permissionDenied());
            }
        } catch (Exception e) {
            return error(RETCODE_SMS_FAIL, e.getMessage());
        }

        return root.toString();
    }
}
