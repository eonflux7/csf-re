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

// Resets the vtable of an embedded C_0082d410 sub-object.
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));

// Game heap free (__cdecl).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Tears down the six embedded C_0082d410 sub-objects of a TMovilAPuntoRastreo
// (at offsets 0x14, 0x10, 0x0C, 0x08, 0x04 and 0x00, in that order), and frees the
// object itself through the game heap when bit 0 of a2 is set. Returns the object pointer.
static char* __fastcall TMovilAPuntoRastreo_Destroy(TMovilAPuntoRastreo* self, void* /*edx*/, char a2)
{
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x004AE8A0, TMovilAPuntoRastreo_Destroy, "auto");
