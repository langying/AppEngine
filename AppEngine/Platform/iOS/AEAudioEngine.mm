//
//  AEAudioEngine.cpp
//  AppEngine
//
//  sound 表示buffers的数组索引，因为是异步生成buffer，所以只能知道索引的位置，不知道索引的内容
//  buffer表示上传音频文件后，内存文件的ID
//  source表示OpenAL中的声源，和buffer是n : 1的关系
//
//  Created by 韩琼 on 16/5/10.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#import "AEPlatform.hpp"
#import "AEAudioEngine.hpp"
#import <AVFoundation/AVFoundation.h>
typedef void(^AEAudioInfoBlock)(ALenum fmt, ALvoid* data, ALsizei size, ALsizei freq);
@interface AEAudioInfo : NSObject
+ (void)setMacOSXMixerOutputRateProc:(ALdouble)value;
+ (void)infoWithURL:(NSString*)pathfile complate:(AEAudioInfoBlock)complete;
+ (void)cafInfoWithURL:(CFURLRef)pathfile complete:(AEAudioInfoBlock)complete;
+ (void)wavInfoWithURL:(CFURLRef)pathfile complete:(AEAudioInfoBlock)complete;
@end
@implementation AEAudioInfo
+ (void)setMacOSXMixerOutputRateProc:(ALdouble)value {
    static alcMacOSXMixerOutputRateProcPtr proc = NULL;
    if (proc == NULL) {
        proc = (alcMacOSXMixerOutputRateProcPtr) alcGetProcAddress(NULL, "alcMacOSXMixerOutputRate");
    }
    if (proc) {
        proc(value);
    }
}
+ (void)infoWithURL:(NSString*)pathfile complate:(AEAudioInfoBlock)complete {
    CFURLRef           url = (__bridge CFURLRef)[NSURL fileURLWithPath:pathfile];
    CFStringRef        ext = CFURLCopyPathExtension(url);
    CFComparisonResult wav = CFStringCompare(ext, (CFStringRef)@"wav", kCFCompareCaseInsensitive);
    if (wav == kCFCompareEqualTo) {
        [AEAudioInfo wavInfoWithURL:url complete:complete];
    }
    else {
        [AEAudioInfo cafInfoWithURL:url complete:complete];
    }
    CFRelease(ext);
    CFRelease(url);
}
+ (void)cafInfoWithURL:(CFURLRef)pathfile complete:(AEAudioInfoBlock)complete {
    ExtAudioFileRef afid = NULL;
    do {
        OSStatus error = ExtAudioFileOpenURL(pathfile, &afid);
        if (error) {
            NSLog(@"ExtAudioFileOpenURL FAILED, Error = %@\n", @(error));
            break;
        }
        UInt32 size = sizeof(AudioStreamBasicDescription);
        AudioStreamBasicDescription fmt;
        error = ExtAudioFileGetProperty(afid, kExtAudioFileProperty_FileDataFormat, &size, &fmt);
        if (error) {
            NSLog(@"ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %@\n", @(error));
            break;
        }
        if (fmt.mChannelsPerFrame > 2) {
            NSLog(@"Unsupported Format, channel count is greater than stereo\n");
            break;
        }
        AudioStreamBasicDescription output;
        output.mSampleRate        = fmt.mSampleRate;
        output.mChannelsPerFrame  = fmt.mChannelsPerFrame;
        output.mFormatID          = kAudioFormatLinearPCM;
        output.mBytesPerPacket    = 2 * output.mChannelsPerFrame;
        output.mFramesPerPacket   = 1;
        output.mBytesPerFrame     = 2 * output.mChannelsPerFrame;
        output.mBitsPerChannel    = 16;
        output.mFormatFlags       = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
        
        error = ExtAudioFileSetProperty(afid, kExtAudioFileProperty_ClientDataFormat, sizeof(output), &output);
        if (error) {
            NSLog(@"ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %@\n", @(error));
            break;
        }
        
        SInt64 frames = 0;
        size  = sizeof(SInt64);
        error = ExtAudioFileGetProperty(afid, kExtAudioFileProperty_FileLengthFrames, &size, &frames);
        if (error) {
            NSLog(@"ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %@\n", @(error));
            break;
        }
        
        UInt32  len = (UInt32)frames * output.mBytesPerFrame;
        ALvoid* buf = calloc(len, 1);
        AudioBufferList bufferList;
        bufferList.mNumberBuffers = 1;
        bufferList.mBuffers[0].mDataByteSize    = len;
        bufferList.mBuffers[0].mNumberChannels  = output.mChannelsPerFrame;
        bufferList.mBuffers[0].mData            = buf;
        
        error = ExtAudioFileRead(afid, (UInt32*)&frames, &bufferList);
        if (error) {
            NSLog(@"ExtAudioFileRead FAILED, Error = %@\n", @(error));
        }
        else {
            complete((output.mChannelsPerFrame > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16), buf, len, output.mSampleRate);
        }
        //            free (buf);
        //            buf = NULL;
    } while(0);
    if (afid) {
        ExtAudioFileDispose(afid);
    }
}
+ (void)wavInfoWithURL:(CFURLRef)pathfile complete:(AEAudioInfoBlock)complete {
    AudioFileID afid = 0;
    do {
        OSStatus error = AudioFileOpenURL(pathfile, kAudioFileReadPermission, 0, &afid);
        if (error) {
            NSLog(@"AudioFileOpenURL FAILED, Error = %@\n", @(error));
            break;
        }
        
        AudioStreamBasicDescription format;
        UInt32 size = sizeof(AudioStreamBasicDescription);
        error = AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &size, &format);
        if (error) {
            NSLog(@"AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %@\n", @(error));
            break;
        }
        if (format.mChannelsPerFrame > 2)  {
            NSLog(@"Unsupported Format, channel count is greater than stereo\n");
            break;
        }
        if (format.mFormatID != kAudioFormatLinearPCM || !TestAudioFormatNativeEndian(format)) {
            NSLog(@"Unsupported Format, must be little-endian PCM\n");;
            break;
        }
        if (format.mBitsPerChannel != 8 && format.mBitsPerChannel != 16) {
            NSLog(@"Unsupported Format, must be 8 or 16 bit PCM\n");
            break;
        }
        
        UInt32 len = 0;
        size = sizeof(UInt32);
        error = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &size, &len);
        if (error) {
            NSLog(@"AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %@\n", @(error));
            break;
        }
        
        ALvoid* data = malloc(len);
        AudioFileReadBytes(afid, false, 0, &len, data);
        if (error) {
            NSLog(@"ExtAudioFileRead FAILED, Error = %@\n", @(error));
        }
        else {
            ALenum fmt;
            if (format.mBitsPerChannel == 16) {
                fmt = format.mChannelsPerFrame > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            }
            else {
                fmt = format.mChannelsPerFrame > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
            }
            complete(fmt, data, len, format.mSampleRate);
        }
        //        free(data);
        //        data = NULL;
    } while (0);
    if (afid) {
        AudioFileClose(afid);
    }
}
@end

float position[] = {0, 0, 0};

AEAudioEngine::~AEAudioEngine() {
    pthread_mutex_lock(&mutex);
    exit = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_join(thid, nullptr);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    
    for (ALint idx = 0; idx < AESourceSize; idx++) {
        ALuint source = sources[idx].sourceId;
        alSourceStop(source);
        alSourcei(source, AL_BUFFER, 0);
        alDeleteSources(1, &source);
    }
    for (ALint idx = 0; idx < AEBufferSize; idx++) {
        ALuint buffer = buffers[idx];
        if (buffer != AEBufferEmpty && buffer != AEBufferLoading) {
            alDeleteBuffers(1, &buffer);
        }
    }
    sounds.clear();
}
AEAudioEngine::AEAudioEngine() {
    exit = false;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 2 * 1024 * 1024);
    pthread_create(&thid, &attr /* nullable */, AEAudioEngine::run, this);
    pthread_cond_init(&cond, nullptr);
    pthread_mutex_init(&mutex, nullptr);
    
    state   = 0;
    error   = 0;
    index   = 0;
    muted   = 0;
    volume  = 1;
    device  = nullptr;
    context = nullptr;
    memset(sources, 0, sizeof(sources));
    for (ALuint idx = 0; idx < AEBufferSize; idx ++) {
        buffers[idx] = AEBufferEmpty;
    }
    add(std::bind(&AEAudioEngine::initOpenAL, this));
}
ALvoid AEAudioEngine::setMuted(ALbool muted) {
    add(std::bind(&AEAudioEngine::syncSetMuted, this, volume));
}
ALvoid AEAudioEngine::setVolume(ALfloat volume) {
    add(std::bind(&AEAudioEngine::syncSetVolume, this, volume));
}
ALvoid AEAudioEngine::init(AEManager *mgr) {
}
ALvoid AEAudioEngine::unload(std::string pathfile) {
    auto it = sounds.find(pathfile);
    if (it != sounds.end()) {
        add(std::bind(&AEAudioEngine::syncUnload, this, it->second));
        sounds.erase(it);
    }
}
ALuint AEAudioEngine::preload(std::string pathfile) {
    auto it = sounds.find(pathfile);
    if (it != sounds.end()) {
        return it->second;
    }
    ALuint ID = cachedBufferIndex();
    sounds[pathfile] = ID;
    add(std::bind(&AEAudioEngine::syncPreload, this, pathfile, ID));
    return ID;
}
ALvoid AEAudioEngine::stopALL() {
    add(std::bind(&AEAudioEngine::syncStopALL, this));
}
ALvoid AEAudioEngine::pauseALL() {
    add(std::bind(&AEAudioEngine::syncPauseALL, this));
}
ALvoid AEAudioEngine::resumeALL() {
    add(std::bind(&AEAudioEngine::syncResumeALL, this));
}
ALuint AEAudioEngine::play(std::string pathfile, ALbool looping) {
    ALuint buffer = buffers[preload(pathfile)];
    ALuint source = sources[cachedSourceIndex()].sourceId;
    add(std::bind(&AEAudioEngine::syncPlay, this, buffer, source, looping));
    return source;
}
ALvoid AEAudioEngine::stop(ALuint source) {
    add(std::bind(&AEAudioEngine::syncStop, this, source));
}
ALvoid AEAudioEngine::pause(ALuint source) {
    add(std::bind(&AEAudioEngine::syncPause, this, source));
}
ALvoid AEAudioEngine::resume(ALuint source) {
    add(std::bind(&AEAudioEngine::syncResume, this, source));
}

ALbool AEAudioEngine::initOpenAL() {
    do {
        device = alcOpenDevice(NULL);
        if (!device) {
            LOG("alcOpenDevice return null.");
            break;
        }
        context = alcCreateContext(device, 0);
        if (context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
            LOG("alcCreateContext return null.");
            if(context) {
                alcDestroyContext(context);
            }
            alcCloseDevice(device);
            break;
        }
        for (ALint idx = 0; idx < AESourceSize; idx++) {
            alGenSources(1, &sources[idx].sourceId);
            alSourcei(sources[idx].sourceId, AL_BUFFER, 0);
        }
        if ((error = alGetError()) == AL_NO_ERROR) {
            return AL_TRUE;
        }
        LOG("init openAL error:%d", error);
    } while (0);
    return AL_FALSE;
}
ALuint AEAudioEngine::cachedBufferIndex() {
    for (ALuint sound = 0; sound < AEBufferSize; sound++) {
        if (buffers[sound] == AEBufferEmpty) {
            buffers[sound] = AEBufferLoading;
            return sound;
        }
    }
    LOG("cached buffer is full used!");
    return 0;
}
ALuint AEAudioEngine::cachedSourceIndex() {
    for (ALuint i = 0; i < AESourceSize; i++) {
        ALuint idx = index++ % AESourceSize;
        alGetSourcei(sources[idx].sourceId, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            return idx;
        }
    }
    LOG("cached source is full used!");
    return index++ % AESourceSize;
}
ALvoid AEAudioEngine::syncSetMuted(ALbool muted) {
    if (this->muted == muted) {
        return;
    }
    this->muted = muted;
    if (muted) {
        alListenerf(AL_GAIN, 0.0f);
    }
    else {
        alListenerf(AL_GAIN, volume);
    }
}
ALvoid AEAudioEngine::syncSetVolume(ALfloat volume) {
    this->volume = volume;
    alListenerf(AL_GAIN, volume);
}
ALvoid AEAudioEngine::syncUnload(ALuint sound) {
    ALuint buffer = buffers[sound];
    do {
        if (buffer == AEBufferEmpty || buffer == AEBufferLoading) {
            break;
        }
        for (ALuint i = 0; i < AESourceSize; i++) {
            if (buffer == sources[i].bufferId) {
                alSourceStop(sources[i].sourceId);
                alSourcei(sources[i].sourceId, AL_BUFFER, 0);
            }
        }
        alDeleteBuffers(1, &buffer);
    } while(0);
    buffers[sound] = AEBufferEmpty;
}
ALvoid AEAudioEngine::syncPreload(std::string pathfile, GLuint sound) {
    [AEAudioInfo infoWithURL:[NSString stringWithUTF8String:pathfile.c_str()] complate:^(ALenum fmt, ALvoid* data, ALsizei size, ALsizei freq) {
        alGenBuffers(1, &buffers[sound]);
        alBufferData(buffers[sound], fmt, data, size, freq);
    }];
}
ALvoid AEAudioEngine::syncStopALL() {
    for (ALint idx = 0; idx < AESourceSize; idx++) {
        alSourceStop(sources[idx].sourceId);
    }
}
ALvoid AEAudioEngine::syncPauseALL() {
    for (ALint idx = 0; idx < AESourceSize; idx++) {
        alSourcePause(sources[idx].sourceId);
    }
}
ALvoid AEAudioEngine::syncResumeALL() {
    for (ALint idx = 0; idx < AESourceSize; idx++) {
        resume(sources[idx].sourceId);
    }
}
ALvoid AEAudioEngine::syncPlay(ALuint sound, ALuint source, ALbool looping) {
    alSourceStop(source);
    alSourcef(source,  AL_GAIN,     1.0);
    alSourcef(source,  AL_PITCH,    1.0);
    alSourcei(source,  AL_BUFFER,   buffers[sound]);
    alSourcei(source,  AL_LOOPING,  looping);
    alSourcefv(source, AL_POSITION, position);
    alSourcePlay(source);
}
ALvoid AEAudioEngine::syncStop(ALuint source) {
    alSourceStop(source);
}
ALvoid AEAudioEngine::syncPause(ALuint source) {
    alSourcePause(source);
}
ALvoid AEAudioEngine::syncResume(ALuint source) {
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state == AL_PAUSED) {
        alSourcePlay(source);
    }
}

void* AEAudioEngine::run(void *self) {
    AEAudioEngine* thiz = static_cast<AEAudioEngine*>(self);
    while (true) {
        pthread_mutex_lock(&thiz->mutex);
        if (thiz->exit){
            break;
        }
        if (thiz->queue.empty()){
            pthread_cond_wait(&thiz->cond, &thiz->mutex);
            pthread_mutex_unlock(&thiz->mutex);
        }
        else {
            auto task = thiz->queue.front();
            thiz->queue.pop();
            pthread_mutex_unlock(&thiz->mutex);
            task();
        }
    }
    return self;
}
void AEAudioEngine::add(const std::function<void()>& task) {
    pthread_mutex_lock(&mutex);
    queue.push(task);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}
