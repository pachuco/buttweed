#pragma once
#include <windows.h>
#include <stdint.h>
#include <assert.h>

//Synth 1: [ ] Panasonic GD87 #1
//Synth 2: [X] Sagem my-v55
//Synth 3: [ ] Panasonic GD87 #2
//Synth 4: [X] NTT DoCoMo
//Synth 5: [X] CMX 3.1.5 lite


typedef enum {
    FAITHERR_OK,
    //....
    
    FAITHERR_ERROR = -1,
} FAITHERR;

typedef enum {
    COMP_UNK,
    COMP_AUDIO_DECODER,
    COMP_SYNTH,
    COMP_FORMAT_PARSER,
    COMP_LSI_SIMULATOR,
    
    NUM_COMPONENTS
} COMPTYPE;

typedef struct {
    DWORD UNK;
    DWORD type;
    DWORD verMaj;
    DWORD verMid;
    DWORD verMin;
    char name[256];
} ComponentInfo;

/////////////////////////////////////////////////////////////////////
//Player1
typedef struct Player1 Player1; 

struct Player1 {
  BYTE unk[620];
};

/////////////////////////////////////////////////////////////////////
//Player2
typedef struct Player2 Player2; 

struct Player2 {
  BYTE unk[668];
};

/////////////////////////////////////////////////////////////////////
//Player3
typedef struct Player3 Player3; 

struct Player3 {
  BYTE unk[620];
};

/////////////////////////////////////////////////////////////////////
//Player4
typedef struct Player4 Player4; 

struct Player4 {
  BYTE unk[672];
};

/////////////////////////////////////////////////////////////////////
//Player5
typedef struct Player5 Player5; 

struct Player5 {
  BYTE unk[672];
};

/////////////////////////////////////////////////////////////////////
//Synth2
#define SYNTH2_RENDERBUF 128

typedef struct Synth2 Synth2;

struct Synth2 {
    void* opaqueBase; //sizeof, 744
    DWORD polyphony;
    DWORD UNK;
    
    CDECL void     (*fpFree)            (Synth2* pSynth);
    CDECL FAITHERR (*fpRender)          (Synth2* pSynth, int32_t pBufInOut[2*128]);
    CDECL int      (*fpUseless1)        (Synth2* pSynth);
    CDECL void     (*fpKindaUseless1)   (Synth2* pSynth);
    CDECL void     (*fpUseless2)        (void);
    CDECL FAITHERR (*fpMidiFeed)        (Synth2* pSynth, int unused, BYTE pDat[4], int numMessages);
    CDECL FAITHERR (*fpSysexFeed)       (Synth2* pSynth, int unused, BYTE pDat[8]);
    CDECL void     (*fpFunc10)          (Synth2* pSynth, int unused, int a3, void* unkStruct);
    CDECL FAITHERR (*fpFunc11)          (Synth2* pSynth, int unused, int zero, BYTE* a4, int one);
};

/*
- opaqueBase
- polyphony
- UNK
- fpFree
- fpRender
    //LR pairs
- fpUseless1
    //return (a1 != 0) + 1; //What?
- fpKindaUseless1
    //called once on playback start. Can be ommited.
- fpUseless2
    //nullsub
- fpMidiFeed
    //this is our midi input
- fpSysexFeed
    //called at song start x4 and at loop x2.
    //a3 contents are:
    //F07F7F04.020040
    //F07F7F04.01007E
- fpFunc10
    //no idea
    //switch(a3) 0x10001 0x10002 0x10003;
- fpFunc11
    //no idea
*/

/////////////////////////////////////////////////////////////////////
//Synth3///////////////There is no synth DLL!////////////////////////


/////////////////////////////////////////////////////////////////////
//Synth4

typedef struct Synth4p1 Synth4p1;
typedef struct Synth4p2 Synth4p2;
typedef struct Synth4   Synth4;

struct Synth4p1 {
    void* opaqueBase; //sizeof, 1088
    DWORD UNK1;
    DWORD UNK2;
    
    CDECL void      (*fpFree)           (Synth4p1* pSynth);
    CDECL FAITHERR  (*fpRender)         (Synth4p1* pSynth, int32_t pBufInOut[2*128]);
    CDECL int       (*fpUseless1)       (Synth4p1* pSynth);
    CDECL FAITHERR  (*fpKindaUseless1)  (Synth4p1* pSynth, int polyphony);
    CDECL int       (*fpFunc7)          (Synth4p1* pSynth, int a2);
    CDECL FAITHERR  (*fpMidiFeed)       (Synth4p1* pSynth, int dunno, BYTE pDat[4], int numMessages);
    CDECL FAITHERR  (*fpSysexFeed)      (Synth4p1* pSynth, int dunno, BYTE* pDat, int numBytes);
    CDECL FAITHERR  (*fpFunc10)         (Synth4p1* pSynth, int a2, int a3, int a4, int a5);
    void* unused;
};

struct Synth4p2 {
    void* opaqueBase; //sizeof, 4380
    DWORD UNK;
    
    CDECL void (*fpFree)        (Synth4p2* pSynth);
    CDECL void (*fpFunc3)       (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc4)       (Synth4p2* pSynth);
    CDECL void (*fpFunc5)       (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc6)       (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc7)       (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc8)       (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc9)       (Synth4p2* pSynth, int a2, int a3, SHORT a4, SHORT a5, int a6, BYTE a7);
    CDECL void (*fpFunc10)      (Synth4p2* pSynth, int a2);
    CDECL void (*fpFunc11)      (Synth4p2* pSynth, int a2, SHORT a3, SHORT a4);
    CDECL void (*fpFunc12)      (Synth4p2* pSynth, int a2, int a3);
    CDECL void (*fpFunc13)      (Synth4p2* pSynth, int a2, BYTE a3);
    CDECL void (*fpFunc14)      (Synth4p2* pSynth, int a2, unsigned int a3);
};

struct Synth4 {
    Synth4p1* ptr1;
    Synth4p2* ptr2;
};

/////////////////////////////////////////////////////////////////////
//Synth5

typedef struct Synth5p1 Synth5p1;
typedef struct Synth5p2 Synth5p2;
typedef struct Synth5 Synth5;

struct Synth5p1 {
    void* opaqueBase; //sizeof, 1088
    DWORD UNK1;
    DWORD UNK2;
    
    CDECL void (*fpFree)        (Synth5p1* pSynth);
    CDECL void (*fpRender)      (Synth5p1* pSynth, int32_t pBufInOut[2*128]);
    CDECL void (*fpUseless1)    (Synth5p1* pSynth);
    CDECL void (*fpFunc6)       (Synth5p1* pSynth, int a2);
    CDECL void (*fpFunc7)       (Synth5p1* pSynth, int a2);
    CDECL void (*fpFunc8)       (Synth5p1* pSynth, int a2, int a3, int a4);
    CDECL void (*fpFunc9)       (Synth5p1* pSynth, int a2, int a3, int a4);
    CDECL void (*fpFunc10)      (Synth5p1* pSynth, int a2, int a3, int a4, int a5);
    void* unused;
};

struct Synth5p2 {
    void* opaqueBase; //sizeof, 144
    DWORD polyphony;
    
    CDECL void (*fpFree)        (Synth5p2* pSynth);
    CDECL FAITHERR (*fpFunc3)   (Synth5p2* pSynth, void* a2); //render?
    CDECL void (*fpFunc4)       (Synth5p2* pSynth); //double deref
    CDECL void (*fpFunc5)       (Synth5p2* pSynth, int voiceIdx); //some other double deref, a2 < 32, voice something
    CDECL void (*fpFunc6)       (Synth5p2* pSynth, int voiceIdx); //same
    CDECL void (*fpFunc7)       (Synth5p2* pSynth, int voiceIdx); //same
    CDECL void (*fpFunc8)       (Synth5p2* pSynth, int voiceIdx); //same
    CDECL void (*fpFunc9)       (Synth5p2* pSynth, signed int a2, BYTE *a3, USHORT a4, USHORT a5, int a6, BYTE a7); //maybe midiFeed
    CDECL void (*fpFunc10)      (Synth5p2* pSynth, int voiceIdx); //voice something
    CDECL void (*fpFunc11)      (Synth5p2* pSynth, int voiceIdx, SHORT a3, SHORT a4); //voice something-else
    CDECL void (*fpFunc12)      (Synth5p2* pSynth, int voiceIdx, DWORD a3); //voice something-else 2
    CDECL void (*fpFunc13)      (Synth5p2* pSynth, int voiceIdx, BYTE a3); //voice something-else 3
    void* unused;
};

struct Synth5 {
    Synth5p1* p1;
    Synth5p2* p2;
};












/////////////////////////////////////////////////////////////////////
// DLL interfaces

// common
CDECL FAITHERR RTPDllGetVersion(ComponentInfo* pSi, DWORD siSize);
// synth dll specific
CDECL void*    RTPSynthOpen(DWORD* type);
CDECL int      RTPSynthClose(void* pPlayer);
// player dll specific
CDECL void*    RTPPlayerOpen(void** ppPlayer);
CDECL FAITHERR RTPPlayerClose(void* pPlayer);
CDECL FAITHERR RTPPlayerPlay(void* pPlayer);
CDECL FAITHERR RTPPlayerStop(void* pPlayer);
CDECL FAITHERR RTPPlayerSeek(void* pPlayer, int a2);
CDECL FAITHERR RTPPlayerGetTime(void* pPlayer);
CDECL FAITHERR RTPPlayerGetState(void* pPlayer);
CDECL FAITHERR RTPPlayerGetExt(void* pPlayer);

#ifdef VALIDATESTRUCTSIZES

static_assert(sizeof(ComponentInfo) == 276, "");

static_assert(sizeof(Player1) == 620, "");
static_assert(sizeof(Player2) == 668, "");
static_assert(sizeof(Player3) == 620, "");
static_assert(sizeof(Player4) == 672, "");
static_assert(sizeof(Player5) == 672, "");

static_assert(sizeof(Synth2) == 48, "");

static_assert(sizeof(Synth4p1) == 48, "");
static_assert(sizeof(Synth4p2) == 60, "");
static_assert(sizeof(Synth4) == 8, "");

static_assert(sizeof(Synth5p1) == 48, "");
static_assert(sizeof(Synth5p2) == 60, "");
static_assert(sizeof(Synth5) == 8, "");

#endif
