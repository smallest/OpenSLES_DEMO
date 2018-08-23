#ifndef OPENGLESDEMO_OPENGLESPlYAER_H
#define OPENGLESDEMO_OPENGLESPlYAER_H

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "smallest", __VA_ARGS__)

class OpenGLESPlayer {
public:
    OpenGLESPlayer();
    void release();
    void prepare();
    void start();
    void setDataSource(File* pcmFile);
private:
    int32_t createEngine();
    int32_t createOutputMix();
    int32_t setDataSource();
    int32_t setDataSink();
private:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf reverb;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    SLObjectItf playerObject;
    SLPlayItf playerPlay;
    SLVolumeItf playerVolume;

    SLAndroidSimpleBufferQueueItf simpleBufferQueue;
    FILE *pcmFile;
    void *buffer;
    uint8_t *out_buffer;

    
};
#endif //OPENGLESDEMO_OPENGLESPlYAER_H
