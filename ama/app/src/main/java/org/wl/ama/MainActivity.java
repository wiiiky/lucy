package org.wl.ama;

import android.app.Activity;
import android.os.Bundle;
import android.os.Debug;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.Scanner;
import java.util.logging.LogRecord;


public class MainActivity extends Activity {

    private TextView textView=null;
    private ServerSocket serverSocket=null;
    private Handler handler=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView=(TextView)findViewById(R.id.tv);

        handler=new Handler();
        startListen();
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

    private String connectionStatus="unknown";

    private void showToastMessage(String s){
        Toast.makeText(MainActivity.this,s,
                Toast.LENGTH_SHORT).show();
    }

    private Runnable showConnectionStatus=new Runnable() {
        @Override
        public void run() {
            showToastMessage(connectionStatus);
        }
    };

    private Runnable socketThread=new Thread(){
        public void run(){
            Socket client=null;
            try{
                serverSocket=new ServerSocket(38300);
                serverSocket.setSoTimeout(10*1000);

                client=serverSocket.accept();
                Scanner socketIn=new Scanner(client.getInputStream());
                PrintWriter socketOut=new PrintWriter(client.getOutputStream());
            }catch (SocketTimeoutException e){
                Log.e("SocketTimeoutException","!");
                connectionStatus=e.getMessage();
            }catch (IOException e){
                Log.e("IOException",e.getMessage());
                connectionStatus=e.getMessage();
            }finally {
                if (serverSocket!=null){
                    try{
                        serverSocket.close();
                    }catch (IOException e){
                        Log.e("IOEception2",e.getMessage());
                        connectionStatus=e.getMessage();
                    }
                }
            }
            if (client==null){
                //connectionStatus="Connection not established!!!";
            }else{
                connectionStatus="Connection established!!!";
            }
            handler.post(showConnectionStatus);
        }

    };

    private void startListen(){
        new Thread(socketThread).start();
    }
}
