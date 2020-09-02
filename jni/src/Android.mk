LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

# Add your application source files here...
LOCAL_SRC_FILES := SDL_android_main.cpp\
main.cpp\

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
