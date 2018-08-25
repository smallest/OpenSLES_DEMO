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
    SLresult createOutputMix();
    SLresult setDataSource();
    SLresult setDataSink();
    void allocateMemory();
private:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf reverb;

    SLObjectItf playerObject;
    SLPlayItf playerPlay;
    SLVolumeItf playerVolume;

    SLAndroidSimpleBufferQueueItf simpleBufferQueue;
    FILE *pcmFile;
    void *buffer;
    uint8_t *out_buffer;

    
};
#endif //OPENGLESDEMO_OPENGLESPlYAER_H
