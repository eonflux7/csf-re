#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static double __stdcall AudioSoundManager_GetGlobalSoundParameter(int parameter)
{
    switch (parameter)
    {
    case 101:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x964);
    case 102:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x968);
    case 103:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x96C);
    case 104:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x970);
    case 105:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x974);
    case 106:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x978);
    case 107:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x97C);
    case 108:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x980);
    case 109:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x984);
    case 110:
        return CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x988);
    default:
        return g_FloatMax;
    }
}

HOOK(0x00525630, AudioSoundManager_GetGlobalSoundParameter, "auto");
