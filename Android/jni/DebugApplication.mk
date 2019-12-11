APP_OPTIM    := debug
APP_STL      := stlport_static
APP_ABI      := armeabi-v7a
APP_PLATFORM := android-17
APP_CFLAGS   += -DNDK_DEBUG -std=c99
APP_CPPFLAGS += -DNDK_DEBUG -std=c++11

NDK_TOOLCHAIN_VERSION := 4.9
