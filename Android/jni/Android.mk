LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE          := AppEngine
LOCAL_MODULE_FILENAME := libAppEngine

MY_SRC_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Engine/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Engine/Core/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Engine/Utils/*cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Library/Duktape/*.c)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Library/Three/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Platform/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/Platform/Android/*.cpp)
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%, %, $(MY_SRC_LIST))

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine/Core
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Engine/Utils
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Library/Duktape
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Library/Three
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Platform
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Platform/Android

LOCAL_LDLIBS   := -llog -landroid -ljnigraphics -lEGL -lGLESv2
LOCAL_CFLAGS   := -Os
LOCAL_CPPFLAGS := -Os

include $(BUILD_SHARED_LIBRARY)
