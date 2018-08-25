#include "jni.h"
#include <android/log.h>
#include "OpenGLESPlayer.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "smallest", __VA_ARGS__)
OpenGLESPlayer *player = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_smallest_test_openglesdemo_MainActivity_testPlayPCM(JNIEnv *env, jobject instance,
                                                             jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);
    player = new OpenGLESPlayer();
    player->setDataSource(filePath);
    player->prepare();
    player->start();
    env->ReleaseStringUTFChars(filePath_, filePath);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_smallest_test_openglesdemo_MainActivity_stop(JNIEnv *env, jobject instance) {
    LOGD("hello jni");
    player->stop();
    player->release();
    delete(player);
    player = NULL;
}
