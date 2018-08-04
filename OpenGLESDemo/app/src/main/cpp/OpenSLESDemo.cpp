#include "OpenGLESDemo.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

SLresult OpenGLESDemo::init() {
    SLObjectItf engineObject;
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
    SLEngineItf engineEngine;
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &(engineEngine));
    if(result != SL_RESULT_SUCCESS) {
        LOGD("engineObject GetInterface failed, result=%d", result);
        return result;
    }
    return result;
}

void OpenGLESDemo::playTest() {

}

void OpenGLESDemo::destory() {

}


