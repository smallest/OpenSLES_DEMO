package com.smallest.test.openglesdemo;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;


public class FileUtils {
    private final static String TAG = FileUtils.class.getSimpleName();
    static public String copyData(Context context, String assetPath, String privateDataPath) {
        Log.d(TAG, "copyData, assetPath=" + assetPath + ", privateDataPath=" + privateDataPath);
        InputStream in = null;
        FileOutputStream out = null;
        String path = context.getFilesDir().getAbsolutePath() + "/" + privateDataPath; // data/data目录
        File file = new File(path);
        if (!file.exists()) {
            try {
                in = context.getAssets().open(assetPath); // 从assets目录下复制
                out = new FileOutputStream(file);
                int length = -1;
                byte[] buf = new byte[1024];
                while ((length = in.read(buf)) != -1) {
                    out.write(buf, 0, length);
                }
                out.flush();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
            finally{
                if (in != null) {
                    try {
                        in.close();
                    } catch (IOException e1) {
                        // TODO Auto-generated catch block
                        e1.printStackTrace();
                    }
                }
                if (out != null) {
                    try {
                        out.close();
                    } catch (IOException e1) {
                        // TODO Auto-generated catch block
                        e1.printStackTrace();
                    }
                }
                return path;
            }
        }
        return null;
    }
}
