#include <stdint.h>
#include "OpenGLESPlayer.h"

FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;
SLAndroidSimpleBufferQueueItf simpleBufferQueue;

int64_t getPcmData(void **pcm, FILE *pcmFile, uint8_t *out_buffer) {
    while(!feof(pcmFile)) {
        size_t size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        *pcm = out_buffer;
        return size;
    }
    return 0;
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void * context) {
    int32_t size = getPcmData(&buffer, pcmFile, out_buffer);
    LOGD("pcmBufferCallBack, size=%d", size);
    if (NULL != buffer && size > 0) {
        SLresult result = (*simpleBufferQueue)->Enqueue(simpleBufferQueue, buffer, size);
    }
}

OpenGLESPlayer::OpenGLESPlayer():
    engineObject(NULL),
    engineEngine(NULL),
    outputMixObject(NULL),
    playerObject(NULL),
    playerPlay(NULL),
    playerVolume(NULL) {
}

SLresult OpenGLESPlayer::createEngine() {
    LOGD("createEngine()"); 
    SLresult result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("slCreateEngine failed, result=%d", result);
        return result;
    }
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("engineObject Realize failed, result=%d", result);
        return result;
    }
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &(engineEngine));
    if(result != SL_RESULT_SUCCESS) {
        LOGD("engineObject GetInterface failed, result=%d", result);
        return result;
    }
    return result;
}

int32_t OpenGLESPlayer::setDataSource(const std::string& path) {
    pcmFile = fopen(path.c_str(), "r");
    if (pcmFile == NULL) {
        LOGD("open path=%s failed.", path.c_str());
        return -1;
    }
    return 0;
}

int32_t OpenGLESPlayer::prepare() {
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);

    //create SLEngineItf
    SLresult ret = createEngine();
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("createEngine() failed.");
        return ret;
    }

    // set DataSource
    SLDataLocator_AndroidSimpleBufferQueue android_queue={SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
    SLDataFormat_PCM sLDataFormat_pcm={
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource slDataSource = {&android_queue, &sLDataFormat_pcm};

    //set DataSink
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    ret = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    if(ret != SL_RESULT_SUCCESS) {
        LOGD("CreateOutputMix failed, ret=%d", ret);
        return ret;
    }
    ret = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(ret != SL_RESULT_SUCCESS) {
        LOGD("Realize failed, result=%d", ret);
        return ret;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};


    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    ret = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &slDataSource, &audioSnk, 1, ids, req);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("CreateAudioPlayer() failed.");
        return ret;
    }
    ret = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject Realize() failed.");
        return ret;
    }
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_PLAY) failed.");
        return ret;
    }
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &simpleBufferQueue);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_BUFFERQUEUE) failed.");
        return ret;
    }

    ret = (*simpleBufferQueue)->RegisterCallback(simpleBufferQueue, pcmBufferCallBack, this);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("SLAndroidSimpleBufferQueueItf RegisterCallback() failed.");
        return ret;
    }
    return ret;
}

void OpenGLESPlayer::start() {
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
//    主动调用回调函数开始工作
    pcmBufferCallBack(simpleBufferQueue, this);
}

void OpenGLESPlayer::stop() {
    LOGD("stop");
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
}

void OpenGLESPlayer::release() {
    if (playerObject != NULL) {
        (*playerObject)->Destroy(playerObject);
        playerObject = NULL;
        playerPlay = NULL;
        playerVolume = NULL;
        simpleBufferQueue = NULL;
        pcmFile = NULL;
        buffer = NULL;
        free(out_buffer);
        out_buffer = NULL;
    }
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
    }

    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}



