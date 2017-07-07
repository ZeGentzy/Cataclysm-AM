LOCAL_PATH := $(call my-dir)

###########################
#
# libintl-lite shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := libintl-lite

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_CPP_FEATURES := exceptions

LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := ../prebuilts/libintl-lite.so

include $(PREBUILT_SHARED_LIBRARY)
