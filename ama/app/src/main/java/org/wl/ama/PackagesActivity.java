package org.wl.ama;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;


public class PackagesActivity extends Activity {

    private ListView listPackages=null;
    private PackageListAdapter packageListAdapter=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_packages);

        ArrayList<PackageListModel> list=new ArrayList<PackageListModel>();
        PackageManager manager=getPackageManager();
        List<PackageInfo> packages=manager.getInstalledPackages(0);
        for (int i=0;i<packages.size();i++){
            PackageInfo info=packages.get(i);
            PackageListModel model=new PackageListModel(
                    info.applicationInfo.loadIcon(manager),
                    info.packageName,
                    info.applicationInfo.loadLabel(manager).toString());
            list.add(model);
        }

        listPackages=(ListView)findViewById(R.id.listPackages);
        packageListAdapter=new PackageListAdapter(this,list);
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
