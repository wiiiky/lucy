package org.wl.ama;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import org.wl.ama.model.ApplicationListModel;

import java.util.ArrayList;
import java.util.List;


public class ApplicationActivity extends Activity {

    private ListView listPackages=null;
    private ApplicationListAdapter packageListAdapter=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_packages);

        ArrayList<ApplicationListModel> list=new ArrayList<ApplicationListModel>();
        PackageManager manager=getPackageManager();
        List<PackageInfo> packages=manager.getInstalledPackages(0);
        for (int i=0;i<packages.size();i++){
            PackageInfo info=packages.get(i);
            ApplicationListModel model=new ApplicationListModel(
                    info.applicationInfo.loadIcon(manager),
                    info.packageName,
                    info.applicationInfo.loadLabel(manager).toString());
            list.add(model);
        }

        listPackages=(ListView)findViewById(R.id.listPackages);
        packageListAdapter=new ApplicationListAdapter(this,list);
        listPackages.setAdapter(packageListAdapter);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.packages, menu);
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

    public void onBackPressed(){
        finish();
        overridePendingTransition(R.anim.in_from_left,R.anim.out_to_right);
    }
}
