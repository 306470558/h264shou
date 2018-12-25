//
// Created by liuxiang on 2017/9/14.
//

#ifndef SCREENLIVE_PACKT_H
#define SCREENLIVE_PACKT_H


#include <string.h>

#include <android/log.h>
#include <malloc.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"RTMP",__VA_ARGS__)

typedef struct {
    int sps_len;
    int pps_len;
    char *sps;
    char *pps;
    char *bufdata;

} Pusher;



void parseVideoConfiguration(char *data, int len, Pusher *pusher) {
    int i=0;
    for (  i = 0; i < len; i++) {
        //0x00 0x00 0x00 0x01
        if (i + 4 < len) {
            if (data[i] == 0x00 && data[i + 1] == 0x00
                && data[i + 2] == 0x00
                && data[i + 3] == 0x01) {
                //0x00 0x00 0x00 0x01 7 sps 0x00 0x00 0x00 0x01 8 pps
                //将sps pps分开
                //找到pps
                if ((data[i + 4] & 0x1f) == 8) {
                    //去掉界定符
                    pusher->sps_len = i - 4;
                    pusher->sps = (char *) malloc(sizeof(char) * pusher->sps_len);
                    memcpy(pusher->sps, data + 4, pusher->sps_len);
                    pusher->pps_len = len - (4 + pusher->sps_len) - 4;
                    pusher->pps = (char *) malloc(sizeof(char) * pusher->pps_len);
                    memcpy(pusher->pps, data + 4 + pusher->sps_len + 4,
                           pusher->pps_len);
                    LOGI("sps:%d pps:%d", pusher->sps_len, pusher->pps_len);
                   /* pusher->bufdata= (char *) malloc(sizeof(char) * pusher->sps_len+sizeof(char) * pusher->pps_len+8);
                    pusher->bufdata[0]= 0x00;
                    pusher->bufdata[1]= 0x00;
                    pusher->bufdata[2]= 0x00;
                    pusher->bufdata[3]= 0x01;
                    memcpy(pusher->bufdata+4, pusher->sps,  pusher->sps_len);
                    int newpo=pusher->sps_len+4;
                    pusher->bufdata[newpo+0]= 0x00;
                    pusher->bufdata[newpo+1]= 0x00;
                    pusher->bufdata[newpo+2]= 0x00;
                    pusher->bufdata[newpo+3]= 0x01;
                    memcpy(pusher->bufdata+newpo+4, pusher->pps,  pusher->pps_len);*/


                    break;
                }
            }
        }
    }
}


#endif //SCREENLIVE_PACKT_H
