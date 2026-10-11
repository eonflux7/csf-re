#include <cstdint>
#include "globals.h"
#include "hook.h"

typedef int (__cdecl* AudioEnumCallback)(int, int, char*, int);

CSF_FUNC(int, __cdecl, AudioEffects_Enumerate, 0x7EE4C0,
         (int* dict, char* name, int mode, AudioEnumCallback callback, int userData));

static int __stdcall Audio_ForwardEnumWithDefaultMode(int dictArg)
{
    return AudioEffects_Enumerate(reinterpret_cast<int*>(dictArg), nullptr, 2, nullptr, 0);
}

HOOK(0x0070ECD0, Audio_ForwardEnumWithDefaultMode, "auto");
