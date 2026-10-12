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
CSF_FUNC(int, __cdecl, ClumpLoader_RegisterAltVariants, 0x006D4320, (int a1, int a2));
CSF_FUNC(int, __cdecl, GfxHAnim_CreateBindPoseAnimation, 0x006E81A0, (int object));

// Registers a clump: creates its bind-pose animation and stores it at +0x30 of the
// owner object. Unless the global flag at byte +4 is set, also runs the alt-variant
// registration for the clump's dictionary entry.
static void __stdcall ClumpLoader_RegisterClump(int a1, int a2)
{
    if (a2)
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));
        CSF_AT(int, owner, 0x30) = GfxHAnim_CreateBindPoseAnimation(a2);

        uint8_t* globals = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalInt()));
        if (!CSF_AT(uint8_t, globals, 4))
        {
            uint8_t* clump = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
            ClumpLoader_RegisterAltVariants(CSF_AT(int, clump, 4), a2);
        }
    }
}

HOOK(0x006D4380, ClumpLoader_RegisterClump, "auto");
