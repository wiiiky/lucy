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

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;


public class MainActivity extends Activity implements View.OnClickListener {

    private TextView tvLog =null;
    private Button btnPackages=null;
    private static ServerSocket serverSocket=null;
    private Handler handler=null;

    /* 监听的端口号 */
    private static int listenPort=37859;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvLog =(TextView)findViewById(R.id.tvLog);
        btnPackages=(Button)findViewById(R.id.btnPackages);

        btnPackages.setOnClickListener(this);

        handler=new Handler();
        startServer();
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

    private String logStr="";
    private Runnable addLog=new Runnable() {
        @Override
        public void run() {
            tvLog.setText(tvLog.getText()+logStr+"\n");
        }
    };

    public void showLog(String log){
        logStr=log;
        handler.post(addLog);
    }

    private static Thread serverThread=null;
    private void startServer(){
        if(serverThread==null||serverThread.isAlive()==false||serverSocket.isClosed()) {
            serverThread=new Thread(serverRunnable);
            serverThread.start();
        }else{
            tvLog.setText("Listening on " + listenPort);
        }
    }

    private Runnable serverRunnable =new Thread(){
        public void run(){
            Socket client;
            try{
                serverSocket=new ServerSocket(listenPort);
                showLog("Listening on "+listenPort);
                while((client = serverSocket.accept())!=null){
                    new ConnectionThread(MainActivity.this,client).start();
                    showLog("Connect to "+client.getRemoteSocketAddress().toString());
                    NotificationHelper.show("PC Connected",client.getRemoteSocketAddress().toString(),MainActivity.this);
                }
            }catch (SocketTimeoutException e){
                showLog("SocketTimeoutException: "+e.getMessage());
            }catch (IOException e){
                showLog("IOException: "+e.getMessage());
            }finally {
                if (serverSocket!=null){
                    try{
                        serverSocket.close();
                    }catch (IOException e){
                        showLog("IOException: "+e.getMessage());
                    }
                }
            }
        }
    };

    @Override
    public void onClick(View view) {
        if(view.getId()==R.id.btnPackages){
            Intent intent=new Intent(this,ApplicationActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.in_from_right,R.anim.out_to_left);
        }
    }
}
