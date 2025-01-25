#pragma once
#include <windows.h>


enum {
    MSG_TYPE_NOTEOFF                = 0x80,
    MSG_TYPE_NOTEON                 = 0x90,
    MSG_TYPE_POLYAFTERTOUCH         = 0xA0,
    MSG_TYPE_PROGCHANGE_CHMODE      = 0xB0,
    MSG_TYPE_PROGRAMCHANGE          = 0xC0,
    MSG_TYPE_CHANNELPRESSURE        = 0xD0,
    MSG_TYPE_PITCHBEND              = 0xE0,
    MSG_TYPE_SYSEX                  = 0xF0,

    FileMetaEvent                   = 0xFF,
};

enum {
    SYS_COMMON                      = 0xF0,
    SYS_EXLUSIVEPACKET              = 0xF7,
    SYS_REALTIME                    = 0xF8,
    SYS_STARTCURRSEQ                = 0xFA,
    SYS_CONTNCURRSEQ                = 0xFB,
    SYS_STOP                        = 0xFC,
};

enum {
    //control changes
    OFF_CCMSB                   = 0x00,
    CCHNG_BANKSELECT                = 0x00,
    CCHNG_MODWHEEL                  = 0x01,
    CCHNG_BREATHCTRL                = 0x02,
    CCHNG_FOOTCTRL                  = 0x04,
    CCHNG_PORTATIME                 = 0x05,
    CCHNG_DATAENTRY                 = 0x06,
    CCHNG_CHANVOL                   = 0x07,
    CCHNG_BALANCE                   = 0x08,
    CCHNG_PAN                       = 0x0A,
    CCHNG_EXPRESSCTRL               = 0x0B,
    CCHNG_FXCTRL1                   = 0x0C,
    CCHNG_FXCTRL2                   = 0x0D,
    CCHNG_GENERALCTRL1              = 0x10,
    CCHNG_GENERALCTRL2              = 0x11,
    CCHNG_GENERALCTRL3              = 0x12,
    CCHNG_GENERALCTRL4              = 0x13,

    OFF_CCLSB                   = 0x20,
    //above repeated

    OFF_CCREST                  = 0x40,
    CCHNG_SUSTAINONOFF              = 0x40,
    CCHNG_PORTAONOFF                = 0x41,
    CCHNG_SOSTENUTONOFF             = 0x42,
    CCHNG_SOFTPEDALONOFF            = 0x43,
    CCHNG_LEGATOFOOTSWITCH          = 0x44,
    CCHNG_HOLD2                     = 0x45,
    CCHNG_SOUNDCTRL1                = 0x46,
    CCHNG_SOUNDCTRL2                = 0x47,
    CCHNG_SOUNDCTRL3                = 0x48,
    CCHNG_SOUNDCTRL4                = 0x49,
    CCHNG_SOUNDCTRL5                = 0x4A,
    CCHNG_SOUNDCTRL6                = 0x4B,
    CCHNG_SOUNDCTRL7                = 0x4C,
    CCHNG_SOUNDCTRL8                = 0x4D,
    CCHNG_SOUNDCTRL9                = 0x4E,
    CCHNG_SOUNDCTRL10               = 0x4F,
    CCHNG_GENERALCTRL5              = 0x50,
    CCHNG_GENERALCTRL6              = 0x51,
    CCHNG_GENERALCTRL7              = 0x52,
    CCHNG_GENERALCTRL8              = 0x53,
    CCHNG_PORTACTRL                 = 0x54,
    CCHNG_HIGHRESVELPREFIX          = 0x58,
    CCHNG_FXDEPTH1                  = 0x5B,
    CCHNG_FXDEPTH2                  = 0x5C,
    CCHNG_FXDEPTH3                  = 0x5D,
    CCHNG_FXDEPTH4                  = 0x5E,
    CCHNG_FXDEPTH5                  = 0x5F,
    CCHNG_DATAINC                   = 0x60,
    CCHNG_DATADEC                   = 0x61,
    CCHNG_NRPNLSB                   = 0x62,
    CCHNG_NRPNMSB                   = 0x63,
    CCHNG_RPNLSB                    = 0x64,
    CCHNG_RPNMSB                    = 0x65,

    //mode changes
    OFF_MC                      = 0x78,
    MCHNG_ALLSOUNDOFF               = 0x78,
    MCHNG_RESETCONTROLERS           = 0x79,
    MCHNG_LOCALONOFF                = 0x7A,
    MCHNG_ALLNOTESOFF               = 0x7B,
    MCHNG_OMNIOFF                   = 0x7C,
    MCHNG_OMNION                    = 0x7D,
    MCHNG_MONOON                    = 0x7E,
    MCHNG_POLYON                    = 0x7F,
};