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

CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __cdecl, RpWorldStreamWrite, 0x0059FD10, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Writes the scene's world object collection to an RenderWare stream opened on handle a1.
// Returns true when the world was written, or when the scene flag at +0x27 is already set.
static bool __stdcall SceneLoader_SaveWorldStream(int a1, C_00863ac0* a2)
{
    if (CSF_AT(uint8_t, a2, 0x27) != 0)
        return true;

    uint32_t* stream = RwStreamOpen(2, 2, a1);
    if (stream == nullptr)
        return false;

    bool written = true;
    int worldObjectCollection = SceneLoader_GetWorldObjectCollection(a2, nullptr);
    if (RpWorldStreamWrite(worldObjectCollection, reinterpret_cast<int>(stream)) == nullptr)
        written = false;

    RwStreamClose(stream, nullptr);
    return written;
}

HOOK(0x007088D0, SceneLoader_SaveWorldStream, "auto");
