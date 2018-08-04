#include "jni.h"
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "smallest", __VA_ARGS__)
extern "C"
JNIEXPORT void JNICALL
Java_com_smallest_test_openglesdemo_MainActivity_test(JNIEnv *env, jobject) {
    LOGD("hello jni");
    return;
}

