
#include <windows.h>

#include "winmm_midi.h"

BOOL midi_init(MidiInDevice* pMid, void* cbMidi, UINT devIndex) {
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