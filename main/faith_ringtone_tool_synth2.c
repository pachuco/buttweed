#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>

typedef struct {
    HMIDIIN hmi;
    MIDIINCAPSA caps;
    MIDIHDR mh;
    INT index;
    BYTE sysexBuf[1024];
} MidiInDevice;

typedef struct {
  CHAR dllName[32];
  typeof(RTPDllGetVersion) fpGetVer;
  typeof(RTPSynthOpen) fpOpen;
  typeof(RTPSynthClose) fpClose;
  Synth2* pInstance;
} Synth2Gubbins;

Synth2Gubbins g_synthGubbins = {"rt_synth_2.dll", 0};

//my guess is, global volume systex.
static BYTE magicJuju1[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x01, 0x00, 0x7E, 0xF7};
static BYTE magicJuju2[8] = {0xF0, 0x7F, 0x7F, 0x04, 0x02, 0x00, 0x40, 0xF7};
static void doInitJuju2(void* pSynth) {
	Synth2* ps2 = (void*)pSynth; 
	
	ps2->fpKindaUseless1(ps2);
	ps2->fpSysexFeed(ps2, 0, magicJuju1);
	ps2->fpSysexFeed(ps2, 0, magicJuju2);
	ps2->fpSysexFeed(ps2, 0, magicJuju1);
	ps2->fpSysexFeed(ps2, 0, magicJuju2);
}




void cbAudioRenderSynth2(void* buffer, int numSamples) {
  int32_t tBuf[SYNTH2_RENDERBUF*2] = {0};
  Synth2* pSynth = g_synthGubbins->pInstance;
  
  assert(numSamples == SYNTH2_RENDERBUF);
  pSynth->fpRender(pSynth, buffer);

  for (int i=0; i<SYNTH2_RENDERBUF*2; i++) ((int16_t*)buffer)[i] = CLAMP(tBuf[i], -32768, 32767);
}

BOOL synth_init(Synth2Gubbins* pSc) {
  DWORD dummy = 0;
  
  pSc->hmodSynth = LoadLibraryA(pSc->dllName);
  pSc->fpGetVer = (void*)GetProcAddress(pSc->hmodSynth, "RTPDllGetVersion");
  pSc->fpOpen   = (void*)GetProcAddress(pSc->hmodSynth, "RTPSynthOpen");
  pSc->fpClose  = (void*)GetProcAddress(pSc->hmodSynth, "RTPSynthClose");
  if (!pSc->hmodSynth || !pSc->fpGetVer || !pSc->fpOpen || !pSc->fpClose) return FALSE;
  
  pSc->pSynth = pSc->fpOpen(&dummy);
  if (!pSc->pSynth) return FALSE;
  
  doInitJuju2(pSc->pSynth);
  
  return TRUE;
}









void CALLBACK cbMidiSynth2(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) { 
  MidiInDevice* m = (MidiInDevice*)dwInstance;
  Synth2* pSynth = g_synthGubbins->pInstance;
  
  switch (wMsg) {
      case MIM_OPEN:
        printf("Opening device: %i: %s\n", m->index+1, m->caps.szPname);
        break;
      case MIM_CLOSE:
        printf("Closing device.\n");
        break;
      case MIM_DATA:
        winmmout_enterCrit();
        pSynth->fpMidiFeed(pSynth, 0, (BYTE*)&dwParam1, 1);
        winmmout_leaveCrit();
        break;
      case MIM_LONGDATA:
      case MIM_LONGERROR:
        midiInUnprepareHeader(m->hmi, &m->mh, sizeof(MIDIHDR));
        
        if (wMsg == MIM_LONGDATA) {
          //&m->mh.dwBytesRecorded ???????????????
          winmmout_enterCrit();
          pSynth->fpSysexFeed(pSynth, 0, &m->sysexBuf);
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



















BOOL midi_init(MidiInDevice* pMid, void* cbMidi, INT devIndex) {
  UINT errMidi = 0;
  
  if (devIndex+1 > midiInGetNumDevs()) return FALSE;
  memset(pMid, 0, sizeof(MidiInDevice));
  
  pMid->index = devIndex;
  pMid->mh.lpData = (LPSTR)&pMid->sysexBuf;
  pMid->mh.dwBufferLength = sizeof(pMid->sysexBuf);
  pMid->mh.dwFlags = 0;
  
  errMidi |= midiInGetDevCapsA(devIndex, &pMid->caps, sizeof(MIDIINCAPSA));
  errMidi |= midiInOpen(&pMid->hmi, devIndex, (DWORD_PTR)cbMidi, (DWORD_PTR)pMid, CALLBACK_FUNCTION);
  errMidi |= midiInPrepareHeader(pMid->hmi, &pMid->mh, sizeof(MIDIHDR));
  errMidi |= midiInAddBuffer(pMid->hmi, &pMid->mh, sizeof(MIDIHDR));
  errMidi |= midiInStart(pMid->hmi);
  return !errMidi;
}

void midi_close(MidiInDevice* pMid) {
  midiInStop(pMid->hmi);
  midiInClose(pMid->hmi);
  midiInUnprepareHeader(pMid->hmi, &pMid->mh, sizeof(MIDIHDR));
  memset(pMid, 0, sizeof(MidiInDevice));
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

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printUsage();
    return 1;
  }
  
  if (!strcmp(argv[1], "-h")) {
    printUsage();
    return 0;
  } else {
    MidiInDevice midev = {0};
    UINT devIndex = strtol(argv[1], NULL, 10);
    
    // ignore if strtol returns 0 because of failure
    if (devIndex == 0 && argv[1][0] != '0') {
      printUsage();
      return 1;
    }
    
    if (!synth_init(&g_lf4)) {
      fprintf(stderr, "Synth init failure!\n");
      return 1;
    }
    
    // rt_synth_2 seems to render 128 samples at a time until I can figure how to set it.
    WinmmFormat wf = {44100, 2, 16,      SYNTH2_RENDERBUF, 16};
    if (!winmmout_openMixer(&wf, &cbAudioRenderSynth2)) {
      fprintf(stderr, "Audio-out init error!\n");
      return 1;
    }

    if (!midi_init(&midev, &cbMidiSynth2, devIndex)) {
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
    midi_close(&midev);
  }

  return 0;
}