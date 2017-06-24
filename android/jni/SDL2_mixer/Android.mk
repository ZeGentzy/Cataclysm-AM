LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2_mixer

# Enable this if you want to support loading MOD music via modplug
# The library path should be a relative path to this directory.
SUPPORT_MOD_MODPLUG ?= false
MODPLUG_LIBRARY_PATH := external/libmodplug-0.8.8.4

# Enable this if you want to support loading MOD music via mikmod
# The library path should be a relative path to this directory.
SUPPORT_MOD_MIKMOD ?= false
MIKMOD_LIBRARY_PATH := external/libmikmod-3.1.12

# Enable this if you want to support loading MP3 music via SMPEG
# The library path should be a relative path to this directory.
SUPPORT_MP3_SMPEG ?= false
SMPEG_LIBRARY_PATH := external/smpeg2-2.0.0

# Enable this if you want to support loading OGG Vorbis music via Tremor
# The library path should be a relative path to this directory.
SUPPORT_OGG ?= true
OGG_LIBRARY_PATH := external/libogg-1.3.1
VORBIS_LIBRARY_PATH := external/libvorbisidec-1.2.1


# Enable this if you want to support TiMidity
SUPPORT_TIMIDITY ?= false

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(ANDROID_TOOLCHAIN_ROOT)/sysroot/usr/include/SDL
LOCAL_CFLAGS := -DWAV_MUSIC

LOCAL_SRC_FILES := ../prebuilts/libSDL2_mixer.so


LOCAL_LDLIBS :=
LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES := SDL2

ifeq ($(SUPPORT_TIMIDITY),true)
	LOCAL_C_INCLUDES += $(LOCAL_PATH)/timidity
	LOCAL_CFLAGS += -DMID_MUSIC -DUSE_TIMIDITY_MIDI
endif

ifeq ($(SUPPORT_MOD_MODPLUG),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(MODPLUG_LIBRARY_PATH)/src $(LOCAL_PATH)/$(MODPLUG_LIBRARY_PATH)/src/libmodplug
    LOCAL_CFLAGS += -DMODPLUG_MUSIC -DMODPLUG_HEADER="<modplug.h>" -DHAVE_SETENV -DHAVE_SINF
endif

ifeq ($(SUPPORT_MOD_MIKMOD),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(MIKMOD_LIBRARY_PATH)/include
    LOCAL_CFLAGS += -DMOD_MUSIC
    LOCAL_SHARED_LIBRARIES += mikmod
endif

ifeq ($(SUPPORT_MP3_SMPEG),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SMPEG_LIBRARY_PATH)
    LOCAL_CFLAGS += -DMP3_MUSIC
    LOCAL_SHARED_LIBRARIES += smpeg2
endif

ifeq ($(SUPPORT_OGG),true)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(OGG_LIBRARY_PATH)/include $(LOCAL_PATH)/$(VORBIS_LIBRARY_PATH)
    LOCAL_CFLAGS += -DOGG_MUSIC -DOGG_USE_TREMOR -DOGG_HEADER="<ivorbisfile.h>"
    ifeq ($(TARGET_ARCH_ABI),armeabi)
	LOCAL_CFLAGS += -D_ARM_ASSEM_
    endif
    ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_CFLAGS += -D_ARM_ASSEM_
    endif
endif

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

LOCAL_ARM_MODE := arm

include $(PREBUILT_SHARED_LIBRARY)
