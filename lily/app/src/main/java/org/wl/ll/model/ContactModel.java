package org.wl.ll.model;

import java.util.ArrayList;

/**
 * Created by wiky on 10/29/14.
 */
public class ContactModel {

    public String id;
    public String name;
    public ArrayList<String> number;

    public ContactModel(String _id,String _name,ArrayList<String> _number){
        id=_id;
        name=_name;
        number=_number;
    }
}
