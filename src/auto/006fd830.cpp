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

CSF_FUNC(int, __fastcall, CollisionShape_ResetVertexBuffer, 0x006D5300, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CollisionShape_ReleaseDerivedState, 0x006D5430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor of C_00862fcc: restores its vtable, resets the collision
// shape's vertex buffer, runs the derived-state cleanup, and frees the object when bit 0 of the flag is set.
static int* __fastcall C_00862fcc_DestroyAndFreeIfFlag(void* self, void* /*edx*/, char a2)
{
    *static_cast<void***>(self) = CSF_VTBL(0x00862FCC);

    CollisionShape_ResetVertexBuffer(self, nullptr);
    CollisionShape_ReleaseDerivedState(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return static_cast<int*>(self);
}

HOOK(0x006FD830, C_00862fcc_DestroyAndFreeIfFlag, "auto");
