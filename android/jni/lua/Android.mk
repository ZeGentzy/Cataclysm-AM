LOCAL_PATH := $(call my-dir)

###########################
#
# Lua shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := lua

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := ../prebuilts/liblua.so

include $(PREBUILT_SHARED_LIBRARY)
