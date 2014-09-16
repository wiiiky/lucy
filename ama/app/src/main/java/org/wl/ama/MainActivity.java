package org.wl.ama;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;


public class MainActivity extends Activity {

    private TextView textView=null;
    private static ServerSocket serverSocket=null;
    private Handler handler=null;

    /* 监听的端口号 */
    private static int listenPort=37859;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView=(TextView)findViewById(R.id.tv);

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
            textView.setText(textView.getText()+logStr+"\n");
        }
    };

    private static Thread serverThread=null;
    private void startServer(){
        if(serverThread==null||serverThread.isAlive()==false||serverSocket.isClosed()) {
            serverThread=new Thread(serverRunnable);
            serverThread.start();
        }
    }

    private Runnable serverRunnable =new Thread(){
        public void run(){
            Socket client=null;
            try{
                serverSocket=new ServerSocket(listenPort);
                logStr="Listening on "+listenPort;
                handler.post(addLog);
                while((client = serverSocket.accept())!=null){
                    new ConnectionThread(client).start();
                    logStr="Connect to "+client.getRemoteSocketAddress().toString();
                    handler.post(addLog);
                    NotificationHelper.show("PC Connected",client.getRemoteSocketAddress().toString(),MainActivity.this);
                }
            }catch (SocketTimeoutException e){
                logStr="SocketTimeoutException: "+e.getMessage();
            }catch (IOException e){
                logStr="IOException: "+e.getMessage();
            }finally {
                if (serverSocket!=null){
                    try{
                        serverSocket.close();
                    }catch (IOException e){
                        logStr="IOException: "+e.getMessage();
                    }
                }
            }
            handler.post(addLog);
        }
    };
}
