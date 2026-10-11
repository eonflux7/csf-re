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

// Destructor-like cleanup of an embedded C_0082d410 sub-object (name chosen here).
CSF_FUNC(void, __fastcall, C_0082d410_Destroy, 0x004DB3C0, (void* self, void* edx));

// Game heap free (__cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down the six embedded C_0082d410 sub-objects of a TNavegando (at offsets
// 0x68, 0x54, 0x3C, 0x28, 0x14 and 0x00, in that order), and frees the object
// itself through the game heap when bit 0 of a2 is set. Returns the object pointer.
static char* __fastcall TNavegando_Destroy(TNavegando* self, void* /*edx*/, char a2)
{
    C_0082d410_Destroy(reinterpret_cast<uint8_t*>(self) + 0x68, nullptr);
    C_0082d410_Destroy(reinterpret_cast<uint8_t*>(self) + 0x54, nullptr);
    C_0082d410_Destroy(reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr);
    C_0082d410_Destroy(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    C_0082d410_Destroy(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    C_0082d410_Destroy(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x00421590, TNavegando_Destroy, "auto");
