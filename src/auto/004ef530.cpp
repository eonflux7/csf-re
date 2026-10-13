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

// Game heap free (0x691110, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Base-layer teardown reached by tail jump (0x771030, __fastcall, ret 0).
CSF_FUNC(int, __fastcall, DestroyBaseLayer, 0x00771030, (void* self, void* edx));

// Releases the owned entry array (count stored in the dword before the data, 32-byte
// elements, each with a vtable-like pointer at +8), resets the vtables, then tail-jumps
// into the base-class teardown.
static int __fastcall C_0083eee4_ReleaseEntryArray(C_0083eee4* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x0083EEE4);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x0083EEE0);
    self->field_0018 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083EEC4));

    uint32_t arrayField = self->field_001c;
    if (arrayField != 0)
    {
        uint8_t* arrayData = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(arrayField));
        int count = *reinterpret_cast<int*>(arrayData - 4);

        for (int i = 0; i < count; ++i)
        {
            uint8_t* element = arrayData + 32 * i;
            *reinterpret_cast<void**>(element + 8) = CSF_VTBL(0x0082D410);
        }

        Mem_Free(reinterpret_cast<void*>(arrayData - 4));
        self->field_001c = 0;
    }

    self->field_0018 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    return DestroyBaseLayer(self, nullptr);
}

HOOK(0x004EF530, C_0083eee4_ReleaseEntryArray, "auto");
