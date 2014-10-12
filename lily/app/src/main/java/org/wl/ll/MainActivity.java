package org.wl.ll;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.wl.ll.activity.ApplicationActivity;
import org.wl.ll.activity.SMSActivity;

import java.util.LinkedList;
import java.util.Queue;


public class MainActivity extends Activity implements View.OnClickListener {

    private TextView tvLog;
    private Button btnApplication;
    private Button btnSMS;
    private Handler handler;

    /* 监听的端口号 */

    private static MainActivity instance=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvLog =(TextView)findViewById(R.id.tvLog);
        btnApplication =(Button)findViewById(R.id.btnApplication);
        btnSMS =(Button)findViewById(R.id.btnSMS);

        btnApplication.setOnClickListener(this);
        btnSMS.setOnClickListener(this);

        handler=new Handler();
        this.startService(new Intent(this,ListenService.class));

        instance=this;
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        NotificationHelper.cancelAll(this);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
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

    private Runnable addLog=new Runnable() {
        @Override
        public void run() {
            String log="";
            synchronized (aqueue) {
                log = aqueue.remove();
            }
            tvLog.setText(tvLog.getText()+log+"\n");
        }
    };

    private Queue<String> aqueue=new LinkedList<String>();

    public void showLog(String log){
        synchronized (aqueue){
            aqueue.add(log);
        }
        handler.post(addLog);
    }


    public static void LOG(String log){
        if(instance!=null){
            instance.showLog(log);
        }
    }



    @Override
    public void onClick(View view) {
        if(view.getId()==R.id.btnApplication){
            tvLog.setText("");
            Intent intent=new Intent(this,ApplicationActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.in_from_right,R.anim.out_to_left);
        }else if(view.getId()==R.id.btnSMS){
            tvLog.setText("");
            Intent intent=new Intent(this, SMSActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.in_from_right,R.anim.out_to_left);
        }
    }
}
