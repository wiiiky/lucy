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
        LinearLayout rootLayout=new LinearLayout(this);
        ScrollView scrollView=new ScrollView(this);
        LinearLayout tvLayout=new LinearLayout(this);
        TextView tv=new TextView(this);
        LinearLayout btnLayout=new LinearLayout(this);
        Button btnAPP=new Button(this);
        Button btnSMS=new Button(this);

        ViewGroup.LayoutParams rootParams=new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        rootLayout.setOrientation(LinearLayout.VERTICAL);
        rootLayout.setLayoutParams(rootParams);

        rootLayout.addView(scrollView);
        scrollView.setFillViewport(true);
        LinearLayout.LayoutParams svParams=new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, 0);
        svParams.weight=1;
        scrollView.setLayoutParams(svParams);

        ScrollView.LayoutParams tvLayoutParams=new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        tvLayout.setLayoutParams(tvLayoutParams);
        tvLayout.setOrientation(LinearLayout.VERTICAL);
        scrollView.addView(tvLayout);

        LinearLayout.LayoutParams tvParams=new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,0);
        tvParams.weight=1;
        tv.setLayoutParams(tvParams);
        tvLayout.addView(tv);

        LinearLayout.LayoutParams btnLayoutParams=new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        btnLayout.setOrientation(LinearLayout.HORIZONTAL);
        btnLayout.setLayoutParams(btnLayoutParams);
        rootLayout.addView(btnLayout);

        LinearLayout.LayoutParams btnAPPParams=new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        btnAPP.setLayoutParams(btnAPPParams);
        btnAPP.setText("Applications");
        btnLayout.addView(btnAPP);

        ViewGroup.MarginLayoutParams btnSMSMargin=new ViewGroup.MarginLayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        btnSMSMargin.setMargins(20,0,0,0);
        LinearLayout.LayoutParams btnSMSParams=new LinearLayout.LayoutParams(btnSMSMargin);
        btnSMS.setLayoutParams(btnSMSParams);
        btnSMS.setText("SMS");
        btnLayout.addView(btnSMS);

        tvLog=tv;
        btnApplication=btnAPP;
        this.btnSMS=btnSMS;

        btnApplication.setOnClickListener(APPClickListener);
        this.btnSMS.setOnClickListener(SMSClickListener);

        return rootLayout;
    }
}
