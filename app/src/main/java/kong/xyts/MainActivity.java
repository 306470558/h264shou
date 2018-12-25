package kong.xyts;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import kong.xyts.application.SysValue;


public class MainActivity extends AppCompatActivity implements RtmpManager.Callback {
    private final int PERMISSION_CODE = 0x12;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RtmpManager.getInstance().setCallback(this);
       /* if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{Manifest.permission.RECORD_AUDIO}, 10);
        }*/
        if (SysValue.api >= Build.VERSION_CODES.M) {
            getAppPermission();
        } else if (SysValue.api >= 21) {
            // getMeidProjection();
        } else {
            //todo 需要root权限或系统签名
            //  ScreenApplication.getInstance().setDisplayManager(((DisplayManager) getSystemService(Context.DISPLAY_SERVICE)));
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        RtmpManager.getInstance().onActivityResult(requestCode, resultCode, data);
    }

    public void stoplive(View view) {
        RtmpManager.getInstance().stopScreenCapture();
    }

    public void startlive(View view) {
        RtmpManager.getInstance().startScreenCapture(this, "rtmp://192.168.15.128:1935/live/test");

        //RtmpManager.getInstance().startScreenCapture(this, "rtmp://send3.douyu
        // .com/live/3251491rcWeNgVtN?wsSecret=6b671a7d166273eb3c29eaf6des2kd34&wsTime=59be2dbc
        // &wsSeek=off");
    }

    //手游直播、监控操作(安全、分析行为)
    @Override
    public void onStatus(int status) {
        if (status == RtmpManager.STATUS_START) {

        } else {

        }
    }

    private void getAppPermission() {
        Log.d("quanxiao","quanxiaoshenqing");
        ActivityCompat.requestPermissions(this,
                new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO},
                PERMISSION_CODE);
    }
}
