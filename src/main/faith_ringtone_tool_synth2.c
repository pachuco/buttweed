#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include "../common/winmm_audio.h"
#include "../common/winmm_midi.h"
#include "../synth_headers/faith_ringtone_tools.h"

typedef struct {
  CHAR dllName[32];
  HMODULE hmodSynth;
  __typeof(RTPDllGetVersion) *fpGetVer;
  __typeof(RTPSynthOpen) *fpOpen;
  __typeof(RTPSynthClose) *fpClose;
  FaithSynth2* pInstance;
} Synth2Gubbins;

#define CLAMP(X, LOW, HIGH) (((X) > (HIGH)) ? (HIGH) : (((X) < (LOW)) ? (LOW) : (X)))


Synth2Gubbins g_synthGubbins = {"lib\\rt_synth_2.dll", 0};

// my guess is, global volume systex.
static void doInitJuju2(void* pInstance) {
	FaithSynth2* ps2 = (void*)pInstance; 
	
  static BYTE magicJuju1[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x01, 0x00, 0x7E, 0xF7};
  static BYTE magicJuju2[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x02, 0x00, 0x40, 0xF7};
	ps2->fpKindaUseless1(ps2);
	ps2->fpSysexFeed(ps2, 0, magicJuju1);
	ps2->fpSysexFeed(ps2, 0, magicJuju2);
	ps2->fpSysexFeed(ps2, 0, magicJuju1);
	ps2->fpSysexFeed(ps2, 0, magicJuju2);
}




void cbAudioRenderSynth2(void* buffer, int numSamples) {
  int32_t tBuf[SYNTH2_RENDERBUF*2] = {0};
  FaithSynth2* pInstance = g_synthGubbins.pInstance;
  
  assert(numSamples == SYNTH2_RENDERBUF);
  pInstance->fpRender(pInstance, tBuf);

  for (int i=0; i<SYNTH2_RENDERBUF*2; i++) ((int16_t*)buffer)[i] = CLAMP(tBuf[i], -32768, 32767);
}

BOOL synth_init(Synth2Gubbins* pSc) {
  DWORD dummy = 0;
  
  pSc->hmodSynth = LoadLibraryA(pSc->dllName);
  pSc->fpGetVer = (void*)GetProcAddress(pSc->hmodSynth, "RTPDllGetVersion");
  pSc->fpOpen   = (void*)GetProcAddress(pSc->hmodSynth, "RTPSynthOpen");
  pSc->fpClose  = (void*)GetProcAddress(pSc->hmodSynth, "RTPSynthClose");
  if (!pSc->hmodSynth || !pSc->fpGetVer || !pSc->fpOpen || !pSc->fpClose) return FALSE;
  
  pSc->pInstance = pSc->fpOpen(&dummy);
  if (!pSc->pInstance) return FALSE;
  
  return TRUE;
}









void CALLBACK cbMidiSynth2(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) { 
  MidiInDevice* m = (MidiInDevice*)dwInstance;
  FaithSynth2* pInstance = g_synthGubbins.pInstance;
  
  switch (wMsg) {
      case MIM_OPEN:
        printf("Opening device: %i: %s\n", m->index, m->caps.szPname);
        winmmout_enterCrit();
        doInitJuju2(pInstance);
        winmmout_leaveCrit();
        break;
      case MIM_CLOSE:
        printf("Closing device.\n");
        break;
      case MIM_DATA:
        winmmout_enterCrit();
        pInstance->fpMidiFeed(pInstance, 0, (BYTE*)&dwParam1, 1);
        winmmout_leaveCrit();
        break;
      case MIM_LONGDATA:
      case MIM_LONGERROR:
        midiInUnprepareHeader(m->hmi, &m->mh, sizeof(MIDIHDR));
        
        if (wMsg == MIM_LONGDATA) {
          //static BYTE sysexResetGM[] = {0xF0,0x7E,0x7F,0x09,0x01,0xF7};
          
          winmmout_enterCrit();
          //if (&m->mh.dwBytesRecorded >= sizeof(sysexResetGM) && !memcmp(&m->sysexBuf, sysexResetGM, sizeof(sysexResetGM))) {
          //  doInitJuju2(pInstance);
          //}
          
          //&m->mh.dwBytesRecorded ???????????????
          pInstance->fpSysexFeed(pInstance, 0, &m->sysexBuf);
          winmmout_leaveCrit();
        }

        midiInPrepareHeader(m->hmi, &m->mh, sizeof(MIDIHDR));
        midiInAddBuffer(m->hmi, &m->mh, sizeof(MIDIHDR));

        break;
      default:
        printf("hmm");
        break;
  }
}






















void printUsage() {
  printf(
    "Usage:\n"
    "sagemator.exe PARAM\n"
    "Parameters:\n"
    "   -h: Show this help.\n"
    "   NUMBER: 1-999; Connect to midi-in device. Press ESC at any time to exit.\n"
  );
}


MidiInDevice g_midev = {0};
int main(int argc, char* argv[]) {
  if (argc != 2) {
    printUsage();
    return 1;
  }
  
  if (!strcmp(argv[1], "-h")) {
    printUsage();
    return 0;
  } else {
    UINT devIndex = strtol(argv[1], NULL, 10);
    
    // ignore if strtol returns 0 because of failure
    if (devIndex == 0 && argv[1][0] != '0') {
      printUsage();
      return 1;
    }
    
    if (!synth_init(&g_synthGubbins)) {
      fprintf(stderr, "Synth init failure!\n");
      return 1;
    }
    
    // rt_synth_2 seems to render 128 samples at a time until I can figure how to set it.
    WinmmFormat wf = {44100, 2, 16,      SYNTH2_RENDERBUF, 16};
    if (!winmmout_openMixer(&wf, &cbAudioRenderSynth2)) {
      fprintf(stderr, "Audio-out init error!\n");
      return 1;
    }
    
    if (!midi_init(&g_midev, &cbMidiSynth2, devIndex)) {
      fprintf(stderr, "Midi-in init error!\n");
      return 1;
    }
    
    // main loop
    while(1) {
      if (kbhit()) {
        unsigned char c = _getch();

        if (c == 0x1B) break; // ESCAPE
      }


      SleepEx(1, TRUE);
    }

    winmmout_closeMixer();
    midi_close(&g_midev);
  }

  return 0;
}