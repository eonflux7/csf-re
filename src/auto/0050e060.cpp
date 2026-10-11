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

// Original 0x0043E010: returns a global object pointer (cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());

// Original 0x004416B0: __fastcall (ecx = self), one stack argument, ret 4.
CSF_FUNC(char, __fastcall, BddLoader_ProcessSceneFile, 0x004416B0, (void* self, void* edx, int a2));

// Loads the Menus/Escenas.fbs scene file via the object returned by GetGlobal8afc7c.
// The original tail-jumps into 0x441950, which pushes the string address and calls
// 0x4416B0 with ecx = result; that stub is reproduced inline here.
static int BddLoader_LoadMenusEscenasFbs()
{
    int globalObjectHandle = GetGlobal8afc7c();
    void* sceneLoaderSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(globalObjectHandle));
    int sceneFileName = reinterpret_cast<int>(CSF_STR(0x00831038, "Menus/Escenas.fbs"));
    return static_cast<int>(BddLoader_ProcessSceneFile(sceneLoaderSelf, nullptr, sceneFileName));
}

HOOK(0x0050E060, BddLoader_LoadMenusEscenasFbs, "auto");
