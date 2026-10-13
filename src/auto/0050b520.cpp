#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

// Stores the requested window mode on the options object, maps it to the
// registry mode index (0 -> 0, 1 -> 1, anything else -> 2), and forwards it to
// the scene loader's object registry through vtable slot 0x50 / 4 = 20.
static int __fastcall OptionsFile_SetWindowMode(void* self, void* /*edx*/, int mode)
{
    CSF_AT(int, self, 0x2C) = mode;

    int registryMode = 2;
    if (mode == 0)
    {
        registryMode = 0;
    }
    else if (mode == 1)
    {
        registryMode = 1;
    }

    void* globalInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* objectRegistry = reinterpret_cast<void*>(
        static_cast<uintptr_t>(SceneLoader_GetObjectRegistry(globalInt, nullptr)));

    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, objectRegistry, 0)[20])(
        objectRegistry, nullptr, registryMode);
}

HOOK(0x0050B520, OptionsFile_SetWindowMode, "auto");
