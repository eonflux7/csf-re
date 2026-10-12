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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x006923A0, (C_0085e65c* self, void* edx));

// Destroys a C_008671d8 object: resets its vtables, tears down the embedded
// sub-object at +4, and frees the block when bit 0 of the flag argument is set.
static uint32_t* __fastcall C_008671d8_Destroy(C_008671d8* self, void* /*edx*/, char a2)
{
    void** embeddedObject = reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 4);

    *(void***)self = CSF_VTBL(0x008671D8);
    *embeddedObject = CSF_VTBL(0x008671D4);
    C_0085e65c_InitVtable(reinterpret_cast<C_0085e65c*>(embeddedObject), nullptr);
    *(void***)self = CSF_VTBL(0x008670CC);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x736FC0, C_008671d8_Destroy, "auto");
