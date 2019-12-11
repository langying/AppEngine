APP_OPTIM    := release
APP_STL      := stlport_static
APP_ABI      := armeabi-v7a
APP_PLATFORM := android-17
APP_CFLAGS   += -Dlog2\(x\)=\(log\(x\)/1.4426950408889634\) -std=c99
APP_CPPFLAGS += -Dlog2\(x\)=\(log\(x\)/1.4426950408889634\) -std=c++11

NDK_TOOLCHAIN_VERSION := 4.9
