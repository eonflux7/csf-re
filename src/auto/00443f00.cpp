#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, Scene_ResetSubsystemB8, 0x00443CA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the scene subsystem's owned buffers and resets its vtables.
static int __fastcall C_0083109c_Destroy(C_0083109c* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083109C);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00831094));

    Scene_ResetSubsystemB8(self, nullptr);

    int result = 0;
    if (self->field_001c != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    uint8_t* arrayBase = reinterpret_cast<uint8_t*>(self->field_000c);
    if (arrayBase != nullptr)
    {
        // Array header: element count stored just before the data; elements are 40 bytes.
        int elementCount = CSF_AT(int, arrayBase, -4);
        uint8_t* cursor = arrayBase + 40 * elementCount + 4;
        if (elementCount - 1 >= 0)
        {
            int remaining = elementCount + 1;
            do
            {
                cursor -= 40;
                --remaining;
                *reinterpret_cast<void***>(cursor) = CSF_VTBL(0x0082D410);
            } while (remaining != 0);
        }
        result = Mem_Free(reinterpret_cast<void*>(arrayBase - 4));
        self->field_000c = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00443F00, C_0083109c_Destroy, "auto");
