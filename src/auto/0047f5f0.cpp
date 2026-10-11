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

// Cleanup routine of the embedded C_00832f18 sub-object at offset 0x88 (name chosen here).
CSF_FUNC(int, __fastcall, C_00832f18_Cleanup, 0x0047E2E0, (void* self, void* edx));

// Destructor-like cleanup of an embedded C_0082d410 sub-object (name chosen here).
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

// Game heap free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down the C_00832f18 sub-object at offset 0x88 and the five embedded
// C_0082d410 sub-objects (at offsets 0x50, 0x3C, 0x28, 0x14 and 0x00, in that order),
// and frees the object itself through the game heap when bit 0 of a2 is set.
// Returns the object pointer.
static char* __fastcall TAtacarRafaga_Destroy(TAtacarRafaga* self, void* /*edx*/, char a2)
{
    C_00832f18_Cleanup(reinterpret_cast<uint8_t*>(self) + 0x88, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x50, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x0047F5F0, TAtacarRafaga_Destroy, "auto");
