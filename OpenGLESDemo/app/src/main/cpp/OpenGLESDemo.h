#ifndef OPENGLESDEMO_OPENGLESDEMO_H
#define OPENGLESDEMO_OPENGLESDEMO_H
#include <android/log.h>
#include <SLES/OpenSLES.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "smallest", __VA_ARGS__)
class OpenGLESDemo {
    static SLresult init();
    static void playTest();
    static void destory();
};
#endif //OPENGLESDEMO_OPENGLESDEMO_H
