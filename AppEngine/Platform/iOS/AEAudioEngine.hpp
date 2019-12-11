//
//  AEAudioEngine.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/10.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEAudioEngine_hpp
#define AEAudioEngine_hpp

#include "AEPlatform.hpp"

#include <queue>
#include <pthread.h>
#include <OpenAL/OpenAL.h>
#define AESourceSize        32
#define AEBufferSize        128
#define AEBufferEmpty       999999
#define AEBufferLoading     0
typedef ALboolean   ALbool;
typedef struct _AEAudioSource {
    ALuint bufferId;
    ALuint sourceId;
} AEAudioSource;
class AEAudioEngine {
    bool            exit;
    pthread_t       thid;
    pthread_cond_t  cond;
    pthread_mutex_t mutex;
    std::queue<std::function<void()>> queue;
    static void* run(void* self);
    void add(const std::function<void()>& task);
    
    ALint  state;
    ALenum error;
    ALuint index;
    ALCdevice*  device;
    ALCcontext* context;
    ALuint buffers[AEBufferSize];
    AEAudioSource sources[AESourceSize];
    std::map<std::string, ALuint> sounds;
    
    ALbool initOpenAL();
    ALuint cachedBufferIndex();
    ALuint cachedSourceIndex();
    ALvoid syncSetMuted(ALbool muted);
    ALvoid syncSetVolume(ALfloat volume);
    ALvoid syncUnload(ALuint sound);
    ALvoid syncPreload(std::string pathfile, GLuint sound);
    ALvoid syncStopALL();
    ALvoid syncPauseALL();
    ALvoid syncResumeALL();
    ALvoid syncPlay(ALuint sound, ALuint source, ALbool looping = AL_FALSE);
    ALvoid syncStop(ALuint source);
    ALvoid syncPause(ALuint source);
    ALvoid syncResume(ALuint source);
    
public:
    ~AEAudioEngine();
    AEAudioEngine();
    ALbool  muted;  ALvoid setMuted(ALbool muted);    ALbool  getMuted() { return muted; };
    ALfloat volume; ALvoid setVolume(ALfloat volume); ALfloat getVolume() { return volume; };
    ALvoid init(AEManager* mgr);
    ALvoid unload(std::string pathfile);
    ALuint preload(std::string pathfile);
    ALvoid stopALL();
    ALvoid pauseALL();
    ALvoid resumeALL();
    ALuint play(std::string pathfile, ALbool looping = AL_FALSE); // return source
    ALvoid stop(ALuint source);
    ALvoid pause(ALuint source);
    ALvoid resume(ALuint source);
};

#endif /* AEAudioEngine_hpp */
