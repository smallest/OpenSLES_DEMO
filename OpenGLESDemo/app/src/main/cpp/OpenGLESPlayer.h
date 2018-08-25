#ifndef OPENGLESDEMO_OPENGLESPlYAER_H
#define OPENGLESDEMO_OPENGLESPlYAER_H

#include <android/log.h>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "smallest", __VA_ARGS__)

class OpenGLESPlayer {
public:
    OpenGLESPlayer();
    int32_t setDataSource(const std::string& pcmPath);
    int32_t prepare();
    void start();
    void stop();
    void release();
private:
    SLresult createEngine();
private:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    SLObjectItf outputMixObject;
    SLObjectItf playerObject;
    SLPlayItf playerPlay;
    SLVolumeItf playerVolume;


};
#endif //OPENGLESDEMO_OPENGLESPlYAER_H
