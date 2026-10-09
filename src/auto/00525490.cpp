#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall AudioSoundManager_GetEventSoundId(void* source, int event)
{
    if (event == 106 && (CSF_AT(uint32_t, source, 0x2E0) & 0x10000) != 0)
        return -1;

    switch (CSF_AT(int, source, 0x54))
    {
    case 1:
        switch (event)
        {
        case 'e': return 69;
        case 'f': return 70;
        case 'g': return 71;
        case 'i': return 72;
        case 'j': return 73;
        case 'n': return 74;
        default: return -1;
        }

    case 2:
        switch (event)
        {
        case 'e': return 83;
        case 'f': return 84;
        case 'j': return 85;
        case 'l': return 86;
        case 'm': return 87;
        case 'n': return 88;
        default: return -1;
        }

    case 3:
        switch (event)
        {
        case 'e': return 75;
        case 'f': return 76;
        case 'g': return 77;
        case 'h': return 78;
        case 'i': return 79;
        case 'j': return 80;
        case 'k': return 81;
        case 'n': return 82;
        default: return -1;
        }

    default:
        return -1;
    }
}

HOOK(0x00525490, AudioSoundManager_GetEventSoundId, "auto");
