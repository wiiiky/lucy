package org.wl.ll.model;

/**
 * Created by wiky on 10/13/14.
 */
public class SMSModel {
    public String body;     //短信内容
    public String date;     //时间
    public String address;  //电话号码

    public SMSModel(String _body, String _date, String _address) {
        body = _body;
        date = _date;
        address = _address;
    }
}
