//
// Created by Administrator on 2018/12/14.
//

/*多线程实现接收多个客户端的文件*/
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>
#include "packt.h"


#define TAG "HELLO"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

#define PORT 8887
#define BUFF_SIZE 1024

typedef struct{
    char type;
    char data[BUFF_SIZE];
}m_package;
void * jianting( );
void* process_client();
int ss ,client_sock,sendres;
 int ret,count,xunhuani,sendlen;
int begeinsend;
Pusher *pusher = 0;

int main(){
    ss = create_tcp_server(PORT);
    if(-1 == ss)
        exit(-1);
    pthread_t pid;
           if(pthread_create(&pid, NULL, jianting, &ss) < 0){
               printf("pthread_create error\n");
           }
   return ss;
}
int   get_client(){
    return client_sock;
}
void *jianting(void *arg){
  while(1){
         //接受客户端连接
          LOGI("pthread_create dengdai");
         printf("pthread_create dengdai\n");
         socklen_t addrlen = sizeof(struct sockaddr);
         struct sockaddr_in client_addr; //客户端地址结构
         client_sock = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
         if(client_sock < 0){
             printf("accept error\n");
              LOGI("accept error\n");
         }
         printf("accept success\n");
 LOGI("accept success\n");

     }
}
int get_client_sock(){
   return client_sock;
}


int sendVideo(char *buf, int len, long tms,int clients ) {

    LOGI("sendVideobuf buf[4]:%x",buf[4]);
  /*  if (buf[4] == 0x67) {//sps pps
        if (pusher && (!pusher->pps || !pusher->sps)) {
            parseVideoConfiguration(buf, len, pusher);
        }
    }else{
        if(begeinsend==0)
        {
            if (pusher && (!pusher->pps || !pusher->sps)) {
                sendres = write(clients, pusher->bufdata, sizeof(pusher->bufdata));
                begeinsend=1;
                return sendres;
            }

        }
    }*/





     count=(len/BUFF_SIZE);
     if(len%BUFF_SIZE >0)
     {
       count++;
     }
    xunhuani=0;
   /* do {
        if(len-xunhuani*BUFF_SIZE>BUFF_SIZE)
        {
            sendlen=BUFF_SIZE;
        }else{
            sendlen=len-xunhuani*BUFF_SIZE;
        }
        write(client_sock, buf+xunhuani*BUFF_SIZE, sendlen);

        xunhuani++;

    } while (xunhuani<count);*/


    for(xunhuani=0;xunhuani<count;xunhuani++)
     {
           if(len-xunhuani*BUFF_SIZE>BUFF_SIZE)
              {
                 sendlen=BUFF_SIZE;
              }else{
              sendlen=len-xunhuani*BUFF_SIZE;
              }
         sendres= write(clients, buf+xunhuani*BUFF_SIZE, sendlen);
       //  LOGI("sendVideobuf sendres:%ld client_sock:%d",sendres,client_sock);
     }
    if(sendres<0 && client_sock >0){
        begeinsend=0;

        close(client_sock);
        client_sock=0;
    }


    return sendres;
}


int sendAudio(char *buf, int len, int type, int tms) {
    int ret;
    do {
      //  RTMPPacket *packet = packetAudioData(buf, len, type, tms, pusher);
       // ret = sendPacket(packet);
    } while (0);
    return ret;
}
/*处理接收给客户端发送图片*/
void sendpic(int newsockfd){
    size_t len;
    //开启子进程
 //   pid_t pid = fork();
 //   LOGD("fasongdaok pid_t:%d",pid);
  //  if (pid > 0) {

        //定义发送信文本信息息
      /*  char *message = "9yyymessageyy";
          len = lenchar(message);
         LOGD("fasongdaok messagelen:%u:",len);
        //发送信息到客户端

        if (send(newsockfd, message, len, 0) == -1) {
            LOGD("fasongdaok send fail:");
            __android_log_print(ANDROID_LOG_INFO, "jni", "send fail");
            //关闭连接
           // close(newsockfd);
            //关闭子进程

        }*/
        //exit(0);
        //关闭连接
    //定义发送手机截图
  //  LOGD("fasongdaok File fq" );
  //  shot_save("/sdcard/screqqqqqqqenshot.png");
    int64_t startime,sendtime,zhuantime,finishtime;
  //  startime=getCurrentTime();
    FILE *fq=NULL;
    char *cmd="screencap -p /sdcard/screenshot.png";
    // char *cmd="uiautomator dump /sdcard/uidump66.xml";
   FILE *pp = popen(cmd, "r");
    //sleep(1);
     pclose(pp);
///////////////////////////////////////
  /*  FILE *pp = popen(cmd, "r"); //建立管道
    if (!pp) {

    }
    char tmp[1024]; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }

    }
    pclose(pp); //关闭管道*/

  ///////////////////////////////////////


   // zhuantime= getCurrentTime();



      /* if( png_to_jpeg("/sdcard/screenshot.png", "/sdcard/screenshot1.jpg", 15) >0)
        {
        exit(1);
       }
    sendtime= getCurrentTime();*/
        if( ( fq = fopen("/sdcard/screenshot1.jpg","rb") ) == NULL ){

                             LOGD("fasongdaok File open" );
                             exit(1);
       }
       //  png_to_jpeg(inputFile, outputFile, 100);


       fseek(fq,0,SEEK_END);     //定位到文件末
       int nFileLen = ftell(fq);       //文件长度
       fseek(fq,0,0);     //定位到文件头
       int falen= 0;
    LOGD("fasongdaok File nFileLen:%d",nFileLen );
        char  buffer[BUFF_SIZE];
       bzero(buffer,BUFF_SIZE);
     while(!feof(fq)){


            len = fread(buffer, 1, BUFF_SIZE, fq);
           //  LOGD("fasongdaok File len:%d",falen );
      //  falen=falen+len;
             if(len != write(newsockfd, buffer, len)){

                 LOGD("fasongdaok File write" );
                 break;
             }
         }

        fclose(fq);

   // finishtime=getCurrentTime();

    printf("jietutime: %lld ms\n", zhuantime-startime);
    printf("zhuanmatutime: %lld ms\n", sendtime-zhuantime);
    printf("finishtime: %lld ms\n", finishtime-sendtime);

    //}

}

//处理客户端程序
void *process_client(void *arg){
    int size = 0, fd, count = 0, sockid = *(int*)arg;
    m_package pac;
    long total = 0, cur = 0;
    //循环接收文件
    while(1) {
        memset(&pac, 0, sizeof(pac));
        size = read(sockid, &pac, sizeof(pac));
        if(size > 0){
            if (pac.type == 1){
                fd = open(pac.data, O_CREAT|O_WRONLY, 0777);
                if(-1 == fd){
                    printf("open file error!\n");
                    continue;
                }
                count = total = cur = 0;
            }
            else if (pac.type == 2){
                cur += write(fd, pac.data, strlen(pac.data));
                if(count++ % 5000 == 0){
                    printf("recv from client < %d > : %.01lf\%\n", sockid, cur * 100.0 / total);
                    count = 0;
                }
            }
            else if (pac.type == 3){
                printf("recv from client < %d > : 100.0\%\n", sockid);
                printf("recv success\n");
                close(fd);
            }
            else if(pac.type == 4){//文件长度
                total = strtol(pac.data, NULL, 10);
                printf("%ld\n", total);
            }
        }else{
            printf("client disconnected\n");
            close(sockid);
            break;
        }
    }
    return 0;
}
