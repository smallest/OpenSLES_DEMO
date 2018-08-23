#include "OpenGLESDemo.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

OpenGLESPlayer::OpenGLESPlayer():
    engineObject(NULL),
    engineEngine(NULL),
    outputMixObject(NULL),
    reverb(NULL),
    playerObject(NULL),
    playerPlay(NULL),
    playerVolume(NULL) {
    createEngine();
}

int32_t OpenGLESPlayer::createEngine() {
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


int32_t OpenGLESPlayer::createOutputMix() {
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
    result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
    if(result != SL_RESULT_SUCCESS) {
        LOGD("SetEnvironmentalReverbProperties failed, result=%d", result);
        return result;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
}

void OpenGLESDemo::playTest() {

}

void OpenGLESDemo::destory() {

}


