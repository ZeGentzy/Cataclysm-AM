LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2_image

# Enable this if you want to support loading JPEG images
# The library path should be a relative path to this directory.
SUPPORT_JPG ?= true
JPG_LIBRARY_PATH := external/jpeg-9

# Enable this if you want to support loading PNG images
# The library path should be a relative path to this directory.
SUPPORT_PNG ?= true
PNG_LIBRARY_PATH := external/libpng-1.6.2

# Enable this if you want to support loading WebP images
# The library path should be a relative path to this directory.
#
# IMPORTANT: In order to enable this must have a symlink in your jni directory to external/libwebp-0.3.0.
SUPPORT_WEBP ?= false
WEBP_LIBRARY_PATH := external/libwebp-0.3.0


LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CFLAGS := -DLOAD_BMP -DLOAD_GIF -DLOAD_LBM -DLOAD_PCX -DLOAD_PNM \
                -DLOAD_TGA -DLOAD_XCF -DLOAD_XPM -DLOAD_XV
LOCAL_CFLAGS += -O3 -fstrict-aliasing -fprefetch-loop-arrays

LOCAL_SRC_FILES := ../prebuilts/libSDL2_image.so

LOCAL_LDLIBS :=
LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := SDL2

ifeq ($(SUPPORT_JPG),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(JPG_LIBRARY_PATH)
    LOCAL_CFLAGS += -DLOAD_JPG
    # We can include the sources directly so the user doesn't have to...
    #LOCAL_STATIC_LIBRARIES += jpeg
    LOCAL_CFLAGS += -DAVOID_TABLES
endif

ifeq ($(SUPPORT_PNG),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(PNG_LIBRARY_PATH)
    LOCAL_CFLAGS += -DLOAD_PNG
    # We can include the sources directly so the user doesn't have to...
    #LOCAL_STATIC_LIBRARIES += png
    LOCAL_LDLIBS += -lz
endif

ifeq ($(SUPPORT_WEBP),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(WEBP_LIBRARY_PATH)/src
    LOCAL_CFLAGS += -DLOAD_WEBP
    LOCAL_STATIC_LIBRARIES += webp
endif

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

include $(PREBUILT_SHARED_LIBRARY)
