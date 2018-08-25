#include "OpenGLESDemo.h"

void getPcmData(void **pcm, File *pcmFile, uint8_t *out_buffer) {
    while(!feof(pcmFile)) {
        fread(out_buffer, 44100 * 2 * 2, 1, pcmFile);
        if(out_buffer == NULL) {
            LOGD("%s", "read end");
            break;
        } else{
            LOGD("%s", "reading");
        }
        *pcm = out_buffer;
        break;
    }
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void * context) {
    OpenGLESPlayer* player = static_cast<OpenGLESPlayer *> (context);
    getPcmData(&(player->buffer), player->pcmFile, player->out_buffer);
    if (NULL != player->buffer) {
        SLresult result;
        result = (*simpleBufferQueue)->Enqueue(simpleBufferQueue, player->buffer, 44100 * 2 * 2);
    }
}

OpenGLESPlayer::OpenGLESPlayer():
    engineObject(NULL),
    engineEngine(NULL),
    outputMixObject(NULL),
    reverb(NULL),
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

SLresult OpenGLESPlayer::createOutputMix() {
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    SLresult result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("CreateOutputMix failed, result=%d", result);
        return result;
    }
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("Realize failed, result=%d", result);
        return result;
    }
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("GetInterface failed, result=%d", result);
        return result;
    }

    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
    result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("SetEnvironmentalReverbProperties failed, result=%d", result);
        return result;
    }
}

SLresult OpenGLESPlayer::setDataSink() {
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};
    return SL_RESULT_SUCCESS;
}

int32_t OpenGLESPlayer::setDataSource(const std::string& path) {
    this->pcmFile = fopen(path, "r");
    if (this->pcmFile == NULL) {
        LOGD("open path=%s failed.", path.c_str())
        return -1;
    }
    return 0;
}

int32_t OpenGLESPlayer::parepare() {
    allocateMemory();
    SLresult ret = createEngine();
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("createEngine() failed.")
        return ret;
    }
    ret = createOutputMix();
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("createOutputMix() failed.")
        return ret;
    }
    ret = setDataSink();
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("setDataSink() failed.")
        return ret;
    }
    SLDataLocator_AndroidSimpleBufferQueue android_queue={SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
    SLDataFormat_PCM sLDataFormat_pcm={
            SL_DATAFORMAT_PCM,
            2,//2个声道
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource slDataSource = {&android_queue, &sLDataFormat_pcm};
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    ret = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &slDataSource, &audioSnk, 3, ids, req);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("CreateAudioPlayer() failed.")
        return ret;
    }
    ret = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject Realize() failed.")
        return ret;
    }
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_PLAY) failed.")
        return ret;
    }
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &simpleBufferQueue);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_BUFFERQUEUE) failed.")
        return ret;
    }

    //缓冲接口回调
    ret = (*simpleBufferQueue)->RegisterCallback(simpleBufferQueue, pcmBufferCallBack2, this);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("SLAndroidSimpleBufferQueueItf RegisterCallback() failed.")
        return ret;
    }
    return 0;
}

void OpenGLESPlayer::start() {
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
//    主动调用回调函数开始工作
    pcmBufferCallBack2(pcmBufferQueue, this);
}

void OpenGLEPlayer::stop() {
    LOGD("stop");//TODO
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
        outputMixEnvironmentalReverb = NULL;
    }

    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}

void OpenGLESPlayer::allocateMemory() {
    this->out_buffer = (uint8_t *) malloc(44100 * 2 * 2);
}


