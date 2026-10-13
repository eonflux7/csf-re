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

// Resets one embedded CharTask base sub-object (__thiscall, no stack args).
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

// Game heap free (__cdecl).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Tears down the six embedded CharTaskBase sub-objects of a TDarObjeto (at offsets
// 0x64, 0x50, 0x3C, 0x28, 0x14 and 0x00, in that order), and frees the object
// itself through the game heap when bit 0 of a2 is set. Returns the object pointer.
static char* __fastcall TDarObjeto_Destroy(TDarObjeto* self, void* /*edx*/, char a2)
{
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x64, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x50, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x004D4E50, TDarObjeto_Destroy, "auto");
