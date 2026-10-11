#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _dsoundGetSpeakerConfiguration, 0x007F0F30, (int a1));

// Maps a DirectSound speaker configuration code to a 1-based audio channel layout index.
// Unrecognized configurations map to 0.
static int __fastcall DSound_MapSpeakerConfiguration(C_00863ba8* self, void* /*edx*/)
{
    const int speakerConfig = _dsoundGetSpeakerConfiguration((int)self->field_0004);

    switch (speakerConfig)
    {
    case 1:
        return 1;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    default:
        return 0;
    }
}

HOOK(0x007108B0, DSound_MapSpeakerConfiguration, "auto");
