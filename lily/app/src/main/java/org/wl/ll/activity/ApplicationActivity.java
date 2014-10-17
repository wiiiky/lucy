package org.wl.ll.activity;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import org.wl.ll.adapter.ApplicationAdapter;
import org.wl.ll.R;
import org.wl.ll.model.ApplicationModel;

import java.util.ArrayList;
import java.util.List;


public class ApplicationActivity extends Activity {

    private ListView listPackages = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_application);

        listPackages = (ListView) findViewById(R.id.list);

        new ReadApplicationTask().execute();
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

    private class ReadApplicationTask extends AsyncTask<Void, Void, ArrayList<ApplicationModel>> {

        @Override
        protected ArrayList<ApplicationModel> doInBackground(Void... voids) {
            return readApplicationList();
        }

        @Override
        protected void onPostExecute(ArrayList<ApplicationModel> list) {
            ApplicationAdapter adapter = new ApplicationAdapter(ApplicationActivity.this, list);
            listPackages.setAdapter(adapter);
        }

        private ArrayList<ApplicationModel> readApplicationList() {
            ArrayList<ApplicationModel> list = new ArrayList<ApplicationModel>();
            PackageManager manager = getPackageManager();
            List<PackageInfo> packages = manager.getInstalledPackages(0);
            for (int i = 0; i < packages.size(); i++) {
                PackageInfo info = packages.get(i);
                ApplicationModel model = new ApplicationModel(
                        info.applicationInfo.loadIcon(manager),
                        info.packageName,
                        info.applicationInfo.loadLabel(manager).toString());
                list.add(model);
            }
            return list;
        }
    }

    public void onBackPressed() {
        moveTaskToBack(true);
        overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
    }
}
