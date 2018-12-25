package kong.xyts;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.util.concurrent.LinkedBlockingQueue;

import kong.xyts.util.SysUtil;

/**
 * Created by xiang on 2017/9/11.
 */

public class RtmpManager {

    private static RtmpManager instance;
    public static final int STATUS_START = 0;
    public static final int STATUS_STOP = 1;

    public static final int STATUS_NO_PREMISSION = -1;
    public static final int STATUS_VIDEO_FAIL = -2;
    public static final int STATUS_AUDIO_FAIL = -3;
    public static final int STATUS_CONNECT_FAIL = -4;
    public static final int STATUS_SEND_DATA_FAIL = -5;
    private static final char[] HEX_CHAR = {'0', '1', '2', '3', '4', '5',
            '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    public interface Callback {
        void onStatus(int status);
    }

    static {
        System.loadLibrary("tcpServer");
    }

    private VideoCodec videoCodec;
    private AudioCodec audioCodec;
    private Callback callback;
    private String url;
    private LinkedBlockingQueue<IFrame> queue;
    private boolean isRecording;
    private MediaProjectionManager mediaProjectionManager;
    private Handler handler;
    private Thread thread;

    private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/lupingww1.264";
    private   BufferedOutputStream outputStream;
      FileOutputStream outStream;


    private RtmpManager() {
        queue = new LinkedBlockingQueue<>();
        handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == STATUS_CONNECT_FAIL || msg.what == STATUS_SEND_DATA_FAIL) {
                    try {
                        thread.join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    if (null != callback)
                        callback.onStatus(msg.what);
                } else if (msg.what == 100) {
                    videoCodec.startCoding();
                    audioCodec.startCoding();
                }
            }
        };
    }



    public static RtmpManager getInstance() {
        if (null == instance)
            instance = new RtmpManager();
        return instance;
    }

    public void setCallback(Callback callback) {
        this.callback = callback;
    }


    protected void addFrame(IFrame frame) {
        if (!isConnect() || !isRecording) return;
        queue.add(frame);
    }


    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 100 && resultCode == Activity.RESULT_OK) {
            MediaProjection mediaProjection = mediaProjectionManager.getMediaProjection
                    (resultCode, data);
            createfile();
            videoCodec = new VideoCodec();
            videoCodec.setMediaProjection(mediaProjection);
            if (!videoCodec.prepare()) {
                videoCodec.stopCoding();
                if (null != callback)
                    callback.onStatus(STATUS_VIDEO_FAIL);
                return;
            }
            audioCodec = new AudioCodec();
            if (!audioCodec.prepare()) {
                audioCodec.stopCoding();
                if (null != callback)
                    callback.onStatus(STATUS_AUDIO_FAIL);
                return;
            }
            isRecording = true;
            thread = new Thread() {
                @Override
                public void run() {
                    if (!startServer()) {
                        handler.sendEmptyMessage(STATUS_CONNECT_FAIL);
                        return;
                    }
                    int clientid;
                    boolean clientiyouxiao;
                    while (isRecording) {
                        clientid=get_client_sock();
                        clientiyouxiao=false;
                      //  Log.d("aaapath","java clientid:"+String.valueOf(clientid));
                        if (clientid <  1) {
                            try {
                                sleep(30);
                            } catch(Exception e){}

                            continue;
                        }else{
                            clientiyouxiao=true;
                            try {
                                byte[] spspps=  SysUtil.getspspps();
                                sendData(spspps, spspps.length, 0, 11,clientid);
                            } catch (Exception e) { }

                        }
                        queue.clear();


                        boolean isSend = true;
                        handler.sendEmptyMessage(100);
                        while (isRecording && isSend && clientiyouxiao) {
                            if (queue.size() <= 0) {
                                continue;
                            }
                            IFrame iFrame = null;
                            try {
                                iFrame = queue.take();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                            if (null == iFrame) {
                                break;
                            }
                            if (iFrame.getBuffer() != null && iFrame.getBuffer().length != 0 && iFrame.getType()==0)
                                  sendddata(iFrame.getBuffer(), iFrame.getBuffer().length, iFrame.getType(), iFrame.getTms());
                                isSend = sendData(iFrame.getBuffer(), iFrame.getBuffer().length, iFrame.getType(), iFrame.getTms(),clientid);
                        //    Log.d("aaapath","java isSend:"+String.valueOf(isSend));
                        }
                    }
                    isRecording = false;
                    queue.clear();
                    disConnect();
                    if (null != audioCodec)
                        audioCodec.stopCoding();
                    if (null != videoCodec)
                        videoCodec.stopCoding();
                    /*if (!isSend) {
                        handler.sendEmptyMessage(STATUS_SEND_DATA_FAIL);
                    }*/
                }
            };
            thread.start();
            if (null != callback)
                callback.onStatus(STATUS_START);
        } else {
            if (null != callback)
                callback.onStatus(STATUS_NO_PREMISSION);
        }
    }

    public void startScreenCapture(Activity activity, String url) {
        this.url = url;
        mediaProjectionManager = (MediaProjectionManager) activity
                .getSystemService
                        (Context.MEDIA_PROJECTION_SERVICE);
        Intent captureIntent = mediaProjectionManager.createScreenCaptureIntent();
        activity.startActivityForResult(captureIntent, 100);
    }

    public void stopScreenCapture() {
        isRecording = false;
        if (null != thread) {
            thread.interrupt();
        }
        callback.onStatus(STATUS_STOP);
    }

    public boolean sendddata(byte[] data, int len, int type, long tms) {
        if(type==0){
         //   Log.d("aaapath",path);
           // Log.d("aaa",String.valueOf(len)+"_sendmsgdddd data" +"_"+bytesToHexFun2(data));
            try {
                if (outputStream != null) { ////////////////保存到文件
                    outputStream.write(data, 0, len);////////////////保存到文件
                }////////////////保存到文件
            }catch (Exception ee){}
        }


        return true;

    }
    public static String bytesToHexFun2(byte[] bytes) {
        char[] buf = new char[bytes.length * 2];
        int index = 0;
        for(byte b : bytes) { // 利用位运算进行转换，可以看作方法一的变种
            buf[index++] = HEX_CHAR[b >>> 4 & 0xf];
            buf[index++] = HEX_CHAR[b & 0xf];
        }

        return new String(buf);
    }

    public  void createfile(){
        File file = new File(path);
        if(file.exists()){
            file.delete();
        }
        try {
            Log.d("aaapath","创建文件" );
            outputStream = new BufferedOutputStream(new FileOutputStream(file));
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    private native boolean startServer();

    private native boolean isConnect();
    private native int get_client_sock();
    private native void disConnect();

    public native boolean sendData(byte[] data, int len, int type, long tms,int clientid);


}
