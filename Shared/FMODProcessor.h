//
//  FMODProcessor.h
//  SongToWaves
//
//  Created by Brett Renfer on 5/23/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofFmodSoundPlayer.h"

bool bFmodInitialized_ = false;
bool bUseSpectrum_ = false;
float fftValues_[8192];			//
float fftInterpValues_[8192];			//
float fftSpectrum_[8192];		// maximum #FMODProcessor is 8192, in fmodex....


// ---------------------  static vars
static FMOD_CHANNELGROUP * channelgroup;
static FMOD_SYSTEM       * sys;

class FMODProcessor {
public:
    
    
    bool isStreaming;
    bool bMultiPlay;
    bool bLoop;
    bool bLoadedOk;
    bool bPaused;
    float pan; // -1 to 1
    float volume; // 0 - 1
    float internalFreq; // 44100 ?
    float speed; // -n to n, 1 = normal, -1 backwards
    unsigned int length; // in samples;
    int numChannels, numBits;
    
    FMOD_RESULT result;
    FMOD_CHANNEL * channel;
    FMOD_SOUND * sound;

    //------------------------------------------------------------
    FMODProcessor(){
        bLoop 			= false;
        bLoadedOk 		= false;
        pan 			= 0.0; // range for oF is -1 to 1
        volume 			= 1.0f;
        internalFreq 	= 44100;
        speed 			= 1;
        bPaused 		= false;
        isStreaming		= false;
    }
    
    ~FMODProcessor(){
        unloadSound();
    }
    
    //---------------------------------------
    // this should only be called once
    void initializeFmod(){
        if(!bFmodInitialized_){
            FMOD_System_Create(&sys);
#ifdef TARGET_LINUX
			FMOD_System_SetOutput(sys,FMOD_OUTPUTTYPE_ALSA);
#endif
            FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL);  //do we want just 32 channels?
            FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
            bFmodInitialized_ = true;
        }
    }
    
    
    //---------------------------------------
    void closeFmod(){
        if(bFmodInitialized_){
            FMOD_System_Close(sys);
            bFmodInitialized_ = false;
        }
    }
    
    //------------------------------------------------------------
    void unloadSound(){
        if (bLoadedOk){
            stop();						// try to stop the sound
            if(!isStreaming)FMOD_Sound_Release(sound);
            bLoadedOk = false;
        }
    }
    
    //------------------------------------------------------------
    bool getIsPlaying(){
        
        if (!bLoadedOk) return false;
        
        int playing = 0;
        FMOD_Channel_IsPlaying(channel, &playing);
        return (playing != 0 ? true : false);
    }
    
    //------------------------------------------------------------
    float getSpeed(){
        return speed;
    }
    
    //------------------------------------------------------------
    float getPan(){
        return pan;
    }
    
    //------------------------------------------------------------
    float getVolume(){
        return volume;
    }
    
    //------------------------------------------------------------
    bool isLoaded(){
        return bLoadedOk;
    }
    
    //------------------------------------------------------------
    void setVolume(float vol){
        if (getIsPlaying() == true){
            FMOD_Channel_SetVolume(channel, vol);
        }
        volume = vol;
    }
    
    //------------------------------------------------------------
    void setPosition(float pct){
        if (getIsPlaying() == true){
            int sampleToBeAt = (int)(length * pct);
            FMOD_Channel_SetPosition(channel, sampleToBeAt, FMOD_TIMEUNIT_PCM);
        }
    }
    
    void setPositionMS(int ms) {
        if (getIsPlaying() == true){
            FMOD_Channel_SetPosition(channel, ms, FMOD_TIMEUNIT_MS);
        }
    }
    
    //------------------------------------------------------------
    float getPosition(){
        if (getIsPlaying() == true){
            unsigned int sampleImAt;
            
            FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_PCM);
            
            float pct = 0.0f;
            if (length > 0){
                pct = sampleImAt / (float)length;
            }
            return pct;
        } else {
            return 0;
        }
    }
    
    //------------------------------------------------------------
    int getPositionMS(){
        if (getIsPlaying() == true){
            unsigned int sampleImAt;
            
            FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_MS);
            
            return sampleImAt;
        } else {
            return 0;
        }
    }
    
    //------------------------------------------------------------
    void setPan(float p){
        pan = p;
        p = ofClamp(p, -1, 1);
        if (getIsPlaying() == true){
            FMOD_Channel_SetPan(channel,p);
        }
    }
    
    
    //------------------------------------------------------------
    void setPaused(bool bP){
        if (getIsPlaying() == true){
            FMOD_Channel_SetPaused(channel,bP);
            bPaused = bP;
        }
    }
    
    
    //------------------------------------------------------------
    void setSpeed(float spd){
        if (getIsPlaying() == true){
			FMOD_Channel_SetFrequency(channel, internalFreq * spd);
        }
        speed = spd;
    }
    
    
    //------------------------------------------------------------
    void setLoop(bool bLp){
        if (getIsPlaying() == true){
            FMOD_Channel_SetMode(channel,  (bLp == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
        }
        bLoop = bLp;
    }
    
    // ----------------------------------------------------------------------------
    void setMultiPlay(bool bMp){
        bMultiPlay = bMp;		// be careful with this...
    }
    
    // ----------------------------------------------------------------------------
    void play(){
        
        // if it's a looping sound, we should try to kill it, no?
        // or else people will have orphan channels that are looping
        if (bLoop == true){
            FMOD_Channel_Stop(channel);
        }
        
        // if the sound is not set to multiplay, then stop the current,
        // before we start another
        if (!bMultiPlay){
            FMOD_Channel_Stop(channel);
        }
        
        FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound, bPaused, &channel);
        
        FMOD_Channel_GetFrequency(channel, &internalFreq);
        FMOD_Channel_SetVolume(channel,volume);
        setPan(pan);
        FMOD_Channel_SetFrequency(channel, internalFreq * speed);
        FMOD_Channel_SetMode(channel,  (bLoop == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
        
        //fmod update() should be called every frame - according to the docs.
        //we have been using fmod without calling it at all which resulted in channels not being able
        //to be reused.  we should have some sort of global update function but putting it here
        //solves the channel bug
        FMOD_System_Update(sys);
        
    }
    
    // ----------------------------------------------------------------------------
    void stop(){
        FMOD_Channel_Stop(channel);
    }
    
    
    // ----------------------------------------------------------------------------
    bool loadSoundAsSample(string fileName, bool stream = false){
        
        fileName = ofToDataPath(fileName);
        
        // fmod uses IO posix internally, might have trouble
        // with unicode paths...
        // says this code:
        // http://66.102.9.104/search?q=cache:LM47mq8hytwJ:www.cleeker.com/doxygen/audioengine__fmod_8cpp-source.html+FSOUND_Sample_Load+cpp&hl=en&ct=clnk&cd=18&client=firefox-a
        // for now we use FMODs way, but we could switch if
        // there are problems:
        
        bMultiPlay = false;
        
        // [1] init fmod, if necessary
        
        initializeFmod();
        
        // [2] try to unload any previously loaded sounds
        // & prevent user-created memory leaks
        // if they call "loadSound" repeatedly, for example
        
        unloadSound();
        
        // [3] load sound
        
        // CREATESAMPLE loads the entire thing into memory
        int fmodFlags =  FMOD_SOFTWARE | FMOD_CREATESAMPLE;
        
        result = FMOD_System_CreateSound(sys, fileName.c_str(),  fmodFlags, NULL, &sound);
        
        if (result != FMOD_OK){
            bLoadedOk = false;
            ofLogError("FMODProcessor") << "loadSound(): could not load \"" << fileName << "\"";
            cout << result << endl;
        } else {
            bLoadedOk = true;
            FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
            FMOD_Sound_GetFormat(sound, NULL, NULL, &numChannels, &numBits);
            isStreaming = stream;
        }
        
        return bLoadedOk;
    }
    
    // ----------------------------------------------------------------------------
    const float div8 = 0.0078125f;			//1./pow(2., 8.-1.);
    const double div16 = 0.00003051757812;	//1./pow(2., 16.-1.);
    const double div32 = 0.00000000046566;	//1./pow(2., 32.-1.);
    
    void intToFloat(float *dest, void *src, long size, int wordlength)
    {
        long wordlengthInBytes = wordlength / 8;
        long numElementsInBuffer = size / wordlengthInBytes;
        long i;
        
        switch (wordlength) {
            case 8:
            {
                signed char *v = (signed char *)src;
                for ( i = 0; i < numElementsInBuffer; i++)
                    dest[i]=(float)v[i] * div8;
            }
                break;
            case 16:
            {
                signed short *v = (signed short *)src;
                for ( i = 0; i < numElementsInBuffer; i++) {
                    dest[i]=(float)v[i] * div16;
                }
            }
                break;
            case 24:
            {
                unsigned char *v = (unsigned char *)src;
                long c = 0;
                for ( i = 0; i < numElementsInBuffer; i++)	{
                    int32_t value = 0;
                    unsigned char *valuePtr = (unsigned char *)&value;
                    
                    valuePtr[0] = 0;
                    valuePtr[1] = v[c]; c++;
                    valuePtr[2] = v[c]; c++;
                    valuePtr[3] = v[c]; c++;
                    
                    dest[i]=(double)value * div32;
                }
            }
                break;
            case 32:
            {
                printf("!!! 32bit files are not fully supported. Trying anyway...\n");
#if 0 /* this correctly plays 32 bit AIFF files but not WAV. Byte swapping bug in FMOD? */
                unsigned char *v = (unsigned char *)src;
                long c = 0;
                for (long i = 0; i < numElementsInBuffer; i++) {
                    int32_t value = 0;
                    unsigned char *valuePtr = (unsigned char *)&value;
                    
                    valuePtr[3] = v[c]; c++;
                    valuePtr[2] = v[c]; c++;
                    valuePtr[1] = v[c]; c++;
                    valuePtr[0] = v[c]; c++;
                    
                    dest[i]=(double)value * div32;
                }
#else /* this correctly plays 32bit WAV files but not AIFF. Byte swapping bug in FMOD? */
                
                int32_t *v = (int32_t *)src;	
                for ( i = 0; i < numElementsInBuffer; i++) {
                    dest[i]=(double)v[i] * div32;
                }
#endif
                
            }
                break;
            default:
                break;
        }
    }
    
    int getLengthInBytes(){
        
        int framesToBytes = numChannels * numBits / 8;
        return length * framesToBytes;
    }
    
    int getSpectrumAtPercentage(float *targetBuffer, float p, float inc ){
        void  *pointer1;
        void  *pointer2;
        unsigned int length1;
        unsigned int length2;
        
        // numFrames = length
        
        unsigned int pos = p * length;
        
        // lock the buffer
        int framesToBytes = numChannels * numBits / 8;
        FMOD_Sound_Lock(sound, pos * framesToBytes, (length * inc) * framesToBytes, &pointer1, &pointer2, &length1, &length2);
        
        // when we're here, pointer1, pointer2, length1 and length2 will be populated.
        // do something with the sound data found in pointer1.
        // the sound data will have a length stored in length1
        
        cout << length1 << ":" << length2 << endl;
        
        if (pointer1){
            intToFloat(targetBuffer, pointer1, length1, numBits);
        }
        
        // unlock the buffer once you're done
        FMOD_Sound_Unlock(sound,pointer1, pointer2, length1, length2);
        return length1;
    }
    
    int getFrameInBytes(){
        return numChannels * numBits / 8;
    }
    
    int getSpectrumAtFrame(float *targetBuffer, float p, int bufferSize ){
        void  *pointer1;
        void  *pointer2;
        unsigned int length1;
        unsigned int length2;
        
        // numFrames = length
        
        unsigned int pos = p * length;
        
        // lock the buffer
        int framesToBytes = numChannels * numBits / 8;
        FMOD_Sound_Lock(sound, pos * framesToBytes, bufferSize, &pointer1, &pointer2, &length1, &length2);
        
        // when we're here, pointer1, pointer2, length1 and length2 will be populated.
        // do something with the sound data found in pointer1.
        // the sound data will have a length stored in length1
        
        cout << length1 << ":" << length2 << endl;
        
        if (pointer1){
            intToFloat(targetBuffer, pointer1, length1, numBits);
        }
        
        // unlock the buffer once you're done
        FMOD_Sound_Unlock(sound,pointer1, pointer2, length1, length2);
        return length1;
    }
    
    int getSpectrumAtFrameLR(float *targetBufferL, float *targetBufferR, float p, int bufferSize ){
        void  *pointer1;
        void  *pointer2;
        unsigned int length1;
        unsigned int length2;
        
        // numFrames = length
        
        int framesToBytes = numChannels * numBits / 8;
        unsigned int lenBytes = length * framesToBytes;
        int count = lenBytes / bufferSize;
        unsigned int pos = p * count * bufferSize;
        
        // lock the buffer
        FMOD_Sound_Lock(sound, pos, bufferSize, &pointer1, &pointer2, &length1, &length2);
        
        // when we're here, pointer1, pointer2, length1 and length2 will be populated.
        // do something with the sound data found in pointer1.
        // the sound data will have a length stored in length1
        
        cout << length1 << ":" << length2 << endl;
        
        if (pointer1){
            for (int i = 0; i < bufferSize; ++i){
                targetBufferL[i] = ((int *)pointer1)[i] >> 16;
                targetBufferR[i] = (((int *)pointer1)[i] << 16) >> 16;
            }
        }
        
        // unlock the buffer once you're done
        FMOD_Sound_Unlock(sound,pointer1, pointer2, length1, length2);
        return length1;
    }
};
