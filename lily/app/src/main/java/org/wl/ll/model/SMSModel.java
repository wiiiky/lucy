package org.wl.ll.model;

/**
 * Created by wiky on 10/13/14.
 */
public class SMSModel {
    public String content;
    public String time;
    public String sender;

    public SMSModel(String _content, String _time, String _sender) {
        content = _content;
        time = _time;
        sender = _sender;
    }
}
