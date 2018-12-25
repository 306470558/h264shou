#include <string.h>
#include <jni.h>
#include <android/log.h>



#define TAG "HELLO"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

static const char *classPathName = "kong/xyts/RtmpManager";
static bool sServerStart = false;
extern "C"{
int main();
int sendVideo(char* buf, int len, long tms,int client );
int sendAudio(char* buf, int len, int type, int tms);
int get_client();
}
jstring native_stringFromJNI( JNIEnv* env, jobject thiz )  {
    LOGI("native_stringFromJNI");
    return env->NewStringUTF( "Hello from JNI!!! !");
}

jboolean native_startServer(JNIEnv *env, jobject thiz) {
    int ret = 0;
    LOGI("sServerStart %d", sServerStart);
    if (!sServerStart) {
//        ret = startLive555RTSPServer();
        ret = main();
        if (ret > 0) {
            LOGI("startLive555RTSPServer success");
            sServerStart = true;
        } else {
            LOGI("RTSPServer start fail");
        }
    } else {
        ret = 1;
        LOGI("RTSPServer have started");
    }
    return sServerStart;
}



bool native_sendData1(JNIEnv *env, jobject instance, jbyteArray data_,
                     jint len, jint type, jlong tms,jint client) {

    jbyte *data = env->GetByteArrayElements(  data_, NULL);


    int ret;
    switch (type) {
        case 0: //video
           // LOGD("ssss:  %o  %d   %x  ",data[4],data[4],data[4]);
            ret = sendVideo((char*)data, len, tms,client);
            break;
        default: //audio
          //  ret = sendAudio((char*)data, len, type, tms);
            break;
    }
    env->ReleaseByteArrayElements(  data_, data, 0);


    if(ret>0)
    {
        return true;
    }
    return false;
}
bool native_isConnect(JNIEnv *env, jobject instance ) {


        return true;

}
int native_get_client_sock(JNIEnv *env, jobject instance ) {

   int client = get_client();
    return client;

}
void native_disConnect(JNIEnv *env, jobject instance ) {




}
///standardType = {"Z": "boolean", "B": "byte", "C": "char", "S": "short", "I": "int", "J": "long",
//"F": "float", "D": "double", "[B": "byte[]", "[C": "char[]", }
static JNINativeMethod methods[] = {

        {"get_client_sock",  "()I",                  (int *)native_get_client_sock},
        {"disConnect",  "()V",                  (void *)native_disConnect},
        {"isConnect",  "()Z",                  (bool*)native_isConnect},
        {"sendData",  "([BIIJI)Z",                  (bool*)native_sendData1},
        {"startServer", "()Z",                  (bool*)native_startServer},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad begin");
    JNIEnv* env = NULL;
    jclass clazz;
    //获取JNI环境对象
    if( vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed\n");
        return JNI_ERR;
    }
    LOGI("JNI_OnLoad JNI环境对象");
    //注册本地方法.Load 目标类
    clazz = env->FindClass(classPathName);
    if (clazz == NULL){
        LOGE("Native registration unable to find class '%s'", classPathName);
        return JNI_ERR;
    }
    LOGI("JNI_OnLoad registration");
    //注册本地native方法



    if(env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGE("ERROR: MediaPlayer native registration failed\n");
        return JNI_ERR;
    }
    LOGI("JNI_OnLoad finish");
    /* success -- return valid version number */
    return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {
    LOGI("JNI_UnLoad");
    sServerStart = false;
}
