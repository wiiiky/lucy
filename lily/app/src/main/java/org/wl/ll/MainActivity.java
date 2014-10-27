package org.wl.ll;

import android.app.ActionBar;
import android.app.Activity;
import android.app.FragmentTransaction;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import org.wl.ll.activity.ApplicationActivity;
import org.wl.ll.activity.SMSActivity;

import java.util.LinkedList;
import java.util.Queue;


public class MainActivity extends Activity {

    private TextView tvLog;
    private Button btnApplication;
    private Button btnSMS;
    private Handler handler;

    /* 监听的端口号 */

    private static MainActivity instance = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(initContentView());

        ActionBar bar=getActionBar();
        if(bar!=null) {
            bar.setIcon(R.drawable.abiword);
        }

        handler = new Handler();
        this.startService(new Intent(this, ListenService.class));

        instance = this;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        NotificationHelper.cancelAll(this);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        menu.add("MenuItem1").setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                showLog("MenuItem1");
                return true;
            }
        });
        menu.add("MenuItem2").setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                showLog("MenuItem2");
                return true;
            }
        });
        menu.add("EXIT").setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                MainActivity.this.finish();
                return true;
            }
        }).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.

        return super.onOptionsItemSelected(item);
    }

    private Runnable addLog = new Runnable() {
        @Override
        public void run() {
            String log = "";
            synchronized (aqueue) {
                log = aqueue.remove();
            }
            tvLog.setText(tvLog.getText() + log + "\n");
        }
    };

    private Queue<String> aqueue = new LinkedList<String>();

    public void showLog(String log) {
        synchronized (aqueue) {
            aqueue.add(log);
        }
        handler.post(addLog);
    }


    public static void LOG(String log) {
        if (instance != null) {
            instance.showLog(log);
        }
    }

    private final View.OnClickListener APPClickListener=new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            tvLog.setText("");
            Intent intent = new Intent(MainActivity.this, ApplicationActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
        }
    };

    private final View.OnClickListener SMSClickListener=new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            tvLog.setText("");
            Intent intent = new Intent(MainActivity.this, SMSActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
        }
    };

    private LinearLayout initContentView(){
        LinearLayout tmp1 = new LinearLayout(this);
        ViewGroup.LayoutParams tmp1Params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT);
        tmp1.setLayoutParams(tmp1Params);
        tmp1.setOrientation(LinearLayout.VERTICAL);

        ScrollView tmp2 = new ScrollView(this);
        LinearLayout.LayoutParams tmp2Params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,0);
        tmp2Params.weight=1;
        tmp2.setLayoutParams(tmp2Params);
        tmp2.setFillViewport(true);
        tmp1.addView(tmp2);

        LinearLayout tmp3 = new LinearLayout(this);
        ScrollView.LayoutParams tmp3Params = new ScrollView.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT);
        tmp3.setLayoutParams(tmp3Params);
        tmp3.setOrientation(LinearLayout.VERTICAL);
        tmp2.addView(tmp3);

        TextView tvLog = new TextView(this);
        LinearLayout.LayoutParams tvLogParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        tvLogParams.weight=1;
        tvLog.setLayoutParams(tvLogParams);
        tmp3.addView(tvLog);

        LinearLayout tmp4 = new LinearLayout(this);
        LinearLayout.LayoutParams tmp4Params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        tmp4.setLayoutParams(tmp4Params);
        tmp4.setOrientation(LinearLayout.HORIZONTAL);
        tmp1.addView(tmp4);

        Button btnApplication = new Button(this);
        LinearLayout.LayoutParams btnApplicationParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        btnApplication.setLayoutParams(btnApplicationParams);
        btnApplication.setText("Application");
        tmp4.addView(btnApplication);

        Button btnSMS = new Button(this);
        ViewGroup.MarginLayoutParams btnSMSMargins = new ViewGroup.MarginLayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        btnSMSMargins.setMargins(20,0,0,0);
        LinearLayout.LayoutParams btnSMSParams = new LinearLayout.LayoutParams(btnSMSMargins);
        btnSMS.setLayoutParams(btnSMSParams);
        btnSMS.setText("SMS");
        tmp4.addView(btnSMS);

        ImageView tmp5 = new ImageView(this);
        ViewGroup.MarginLayoutParams tmp5Margins = new ViewGroup.MarginLayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,ViewGroup.LayoutParams.WRAP_CONTENT);
        tmp5Margins.setMargins(50,0,0,0);
        LinearLayout.LayoutParams tmp5Params = new LinearLayout.LayoutParams(tmp5Margins);
        tmp5.setLayoutParams(tmp5Params);
        tmp5.setImageDrawable(getResources().getDrawable(R.drawable.ic_launcher));
        tmp4.addView(tmp5);

        this.tvLog=tvLog;
        this.btnApplication=btnApplication;
        this.btnSMS=btnSMS;

        this.btnApplication.setOnClickListener(APPClickListener);
        this.btnSMS.setOnClickListener(SMSClickListener);

        return tmp1;
    }
}
