package com.smallest.test.openglesdemo;

import android.Manifest;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.tbruyelle.rxpermissions2.Permission;
import com.tbruyelle.rxpermissions2.RxPermissions;

import io.reactivex.functions.Consumer;

public class MainActivity extends AppCompatActivity {

    private boolean mHasWriteStoragePermission = false;
    private String TAG = MainActivity.class.getSimpleName();

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestPermission();
        test();
    }

    private void requestPermission(){
        RxPermissions rxPermissions = new RxPermissions(this);

        rxPermissions.requestEach(
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .subscribe(new Consumer<Permission>() {
                    @Override
                    public void accept(Permission permission) throws Exception {
                        if (permission.granted) {
                            mHasWriteStoragePermission = true;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsGranted() : " + permission.name +
                                    " request granted , to do something...");

                        } else if (permission.shouldShowRequestPermissionRationale) {
                            mHasWriteStoragePermission = false;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsDenied() : " + permission.name + "request denied");
                            Toast.makeText(MainActivity.this, "拒绝权限，等待下次询问哦", Toast.LENGTH_SHORT).show();
                        } else {
                            mHasWriteStoragePermission = false;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsDenied() : this " + permission.name + " is denied " +
                                    "and never ask again");
                            Toast.makeText(MainActivity.this, "拒绝权限，不再弹出询问框，请前往APP应用设置中打开此权限", Toast.LENGTH_SHORT).show();
                        }
                    }
                });

    }

    private native void test();
}
