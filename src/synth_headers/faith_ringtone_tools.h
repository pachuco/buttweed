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
  FAITH_ERROR_OK,
  //....
  
  FAITH_ERROR_ERROR = -1,
} FAITH_ERROR;

typedef enum {
  FAITH_TYPE_UNK,
  FAITH_TYPE_AUDIO_DECODER,
  FAITH_TYPE_SYNTH,
  FAITH_TYPE_FORMAT_PARSER,
  FAITH_TYPE_LSI_SIMULATOR,
  
  NUM_COMPONENTS
} FAITH_COMPTYPE;

typedef struct {
  DWORD UNK;
  DWORD type;
  DWORD verMaj;
  DWORD verMid;
  DWORD verMin;
  char name[256];
} FaithComponentInfo;

/////////////////////////////////////////////////////////////////////
//Player1
typedef struct FaithPlayer1 FaithPlayer1; 

struct FaithPlayer1 {
  BYTE unk[620];
};

/////////////////////////////////////////////////////////////////////
//Player2
typedef struct FaithPlayer2 FaithPlayer2; 

struct FaithPlayer2 {
  BYTE unk[668];
};

/////////////////////////////////////////////////////////////////////
//Player3
typedef struct FaithPlayer3 FaithPlayer3; 

struct FaithPlayer3 {
  BYTE unk[620];
};

/////////////////////////////////////////////////////////////////////
//Player4
typedef struct FaithPlayer4 FaithPlayer4; 

struct FaithPlayer4 {
  BYTE unk[672];
};

/////////////////////////////////////////////////////////////////////
//Player5
typedef struct FaithPlayer5 FaithPlayer5; 

struct FaithPlayer5 {
  BYTE unk[672];
};

/////////////////////////////////////////////////////////////////////
//Synth2
#define SYNTH2_RENDERBUF 128

typedef struct FaithSynth2 FaithSynth2;

struct FaithSynth2 {
  void* opaqueBase; //sizeof, 744
  DWORD polyphony;
  DWORD UNK;
  
  CDECL void     (*fpFree)            (FaithSynth2* pSynth);
  CDECL FAITH_ERROR (*fpRender)          (FaithSynth2* pSynth, int32_t pBufInOut[2*128]);
  CDECL int      (*fpUseless1)        (FaithSynth2* pSynth);
  CDECL void     (*fpKindaUseless1)   (FaithSynth2* pSynth);
  CDECL void     (*fpUseless2)        (void);
  CDECL FAITH_ERROR (*fpMidiFeed)        (FaithSynth2* pSynth, int unused, BYTE pDat[4], int numMessages);
  CDECL FAITH_ERROR (*fpSysexFeed)       (FaithSynth2* pSynth, int unused, BYTE pDat[8]);
  CDECL void     (*fpFunc10)          (FaithSynth2* pSynth, int unused, int a3, void* unkStruct);
  CDECL FAITH_ERROR (*fpFunc11)          (FaithSynth2* pSynth, int unused, int zero, BYTE* a4, int one);
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

typedef struct FaithSynth4p1 FaithSynth4p1;
typedef struct FaithSynth4p2 FaithSynth4p2;
typedef struct FaithSynth4   FaithSynth4;

struct FaithSynth4p1 {
  void* opaqueBase; //sizeof, 1088
  DWORD UNK1;
  DWORD UNK2;
  
  CDECL void      (*fpFree)           (FaithSynth4p1* pSynth);
  CDECL FAITH_ERROR  (*fpRender)         (FaithSynth4p1* pSynth, int32_t pBufInOut[2*128]);
  CDECL int       (*fpUseless1)       (FaithSynth4p1* pSynth);
  CDECL FAITH_ERROR  (*fpKindaUseless1)  (FaithSynth4p1* pSynth, int polyphony);
  CDECL int       (*fpFunc7)          (FaithSynth4p1* pSynth, int a2);
  CDECL FAITH_ERROR  (*fpMidiFeed)       (FaithSynth4p1* pSynth, int dunno, BYTE pDat[4], int numMessages);
  CDECL FAITH_ERROR  (*fpSysexFeed)      (FaithSynth4p1* pSynth, int dunno, BYTE* pDat, int numBytes);
  CDECL FAITH_ERROR  (*fpFunc10)         (FaithSynth4p1* pSynth, int a2, int a3, int a4, int a5);
  void* unused;
};

struct FaithSynth4p2 {
  void* opaqueBase; //sizeof, 4380
  DWORD UNK;
  
  CDECL void (*fpFree)        (FaithSynth4p2* pSynth);
  CDECL void (*fpFunc3)       (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc4)       (FaithSynth4p2* pSynth);
  CDECL void (*fpFunc5)       (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc6)       (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc7)       (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc8)       (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc9)       (FaithSynth4p2* pSynth, int a2, int a3, SHORT a4, SHORT a5, int a6, BYTE a7);
  CDECL void (*fpFunc10)      (FaithSynth4p2* pSynth, int a2);
  CDECL void (*fpFunc11)      (FaithSynth4p2* pSynth, int a2, SHORT a3, SHORT a4);
  CDECL void (*fpFunc12)      (FaithSynth4p2* pSynth, int a2, int a3);
  CDECL void (*fpFunc13)      (FaithSynth4p2* pSynth, int a2, BYTE a3);
  CDECL void (*fpFunc14)      (FaithSynth4p2* pSynth, int a2, unsigned int a3);
};

struct FaithSynth4 {
  FaithSynth4p1* p1;
  FaithSynth4p2* p2;
};

/////////////////////////////////////////////////////////////////////
//Synth5

typedef struct FaithSynth5p1 FaithSynth5p1;
typedef struct FaithSynth5p2 FaithSynth5p2;
typedef struct FaithSynth5   FaithSynth5;

struct FaithSynth5p1 {
  void* opaqueBase; //sizeof, 1088
  DWORD UNK1;
  DWORD UNK2;
  
  CDECL void (*fpFree)        (FaithSynth5p1* pSynth);
  CDECL void (*fpRender)      (FaithSynth5p1* pSynth, int32_t pBufInOut[2*128]);
  CDECL void (*fpUseless1)    (FaithSynth5p1* pSynth);
  CDECL void (*fpFunc6)       (FaithSynth5p1* pSynth, int a2);
  CDECL void (*fpFunc7)       (FaithSynth5p1* pSynth, int a2);
  CDECL void (*fpFunc8)       (FaithSynth5p1* pSynth, int a2, int a3, int a4);
  CDECL void (*fpFunc9)       (FaithSynth5p1* pSynth, int a2, int a3, int a4);
  CDECL void (*fpFunc10)      (FaithSynth5p1* pSynth, int a2, int a3, int a4, int a5);
  void* unused;
};

struct FaithSynth5p2 {
  void* opaqueBase; //sizeof, 144
  DWORD polyphony;
  
  CDECL void (*fpFree)        (FaithSynth5p2* pSynth);
  CDECL FAITH_ERROR (*fpFunc3)   (FaithSynth5p2* pSynth, void* a2); //render?
  CDECL void (*fpFunc4)       (FaithSynth5p2* pSynth); //double deref
  CDECL void (*fpFunc5)       (FaithSynth5p2* pSynth, int voiceIdx); //some other double deref, a2 < 32, voice something
  CDECL void (*fpFunc6)       (FaithSynth5p2* pSynth, int voiceIdx); //same
  CDECL void (*fpFunc7)       (FaithSynth5p2* pSynth, int voiceIdx); //same
  CDECL void (*fpFunc8)       (FaithSynth5p2* pSynth, int voiceIdx); //same
  CDECL void (*fpFunc9)       (FaithSynth5p2* pSynth, signed int a2, BYTE *a3, USHORT a4, USHORT a5, int a6, BYTE a7); //maybe midiFeed
  CDECL void (*fpFunc10)      (FaithSynth5p2* pSynth, int voiceIdx); //voice something
  CDECL void (*fpFunc11)      (FaithSynth5p2* pSynth, int voiceIdx, SHORT a3, SHORT a4); //voice something-else
  CDECL void (*fpFunc12)      (FaithSynth5p2* pSynth, int voiceIdx, DWORD a3); //voice something-else 2
  CDECL void (*fpFunc13)      (FaithSynth5p2* pSynth, int voiceIdx, BYTE a3); //voice something-else 3
  void* unused;
};

struct FaithSynth5 {
  FaithSynth5p1* p1;
  FaithSynth5p2* p2;
};












/////////////////////////////////////////////////////////////////////
// DLL interfaces

// common
CDECL FAITH_ERROR   RTPDllGetVersion(FaithComponentInfo* pSi, DWORD siSize);
// synth dll specific
CDECL void*         RTPSynthOpen(DWORD* type);
CDECL int           RTPSynthClose(void* pPlayer);
// player dll specific
CDECL void*         RTPPlayerOpen(void** ppPlayer);
CDECL FAITH_ERROR   RTPPlayerClose(void* pPlayer);
CDECL FAITH_ERROR   RTPPlayerPlay(void* pPlayer);
CDECL FAITH_ERROR   RTPPlayerStop(void* pPlayer);
CDECL FAITH_ERROR   RTPPlayerSeek(void* pPlayer, int a2);
CDECL FAITH_ERROR   RTPPlayerGetTime(void* pPlayer);
CDECL FAITH_ERROR   RTPPlayerGetState(void* pPlayer);
CDECL FAITH_ERROR   RTPPlayerGetExt(void* pPlayer);

#ifdef VALIDATESTRUCTSIZES

static_assert(sizeof(FaithComponentInfo) == 276, "");

static_assert(sizeof(FaithPlayer1) == 620, "");
static_assert(sizeof(FaithPlayer2) == 668, "");
static_assert(sizeof(FaithPlayer3) == 620, "");
static_assert(sizeof(FaithPlayer4) == 672, "");
static_assert(sizeof(FaithPlayer5) == 672, "");

static_assert(sizeof(FaithSynth2) == 48, "");

static_assert(sizeof(FaithSynth4p1) == 48, "");
static_assert(sizeof(FaithSynth4p2) == 60, "");
static_assert(sizeof(FaithSynth4) == 8, "");

static_assert(sizeof(FaithSynth5p1) == 48, "");
static_assert(sizeof(FaithSynth5p2) == 60, "");
static_assert(sizeof(FaithSynth5) == 8, "");

#endif
