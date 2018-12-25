LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


include $(CLEAR_VARS)
LOCAL_MODULE    := tcpServer
LOCAL_SRC_FILES :=threaServer.c \
CreateServer.c \
myMain.cpp \


LOCAL_LDLIBS :=  -llog


include $(BUILD_SHARED_LIBRARY)