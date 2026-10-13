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

CSF_FUNC(int, __fastcall, C_0083109c_ResetSubsystemB8, 0x00443CA0, (C_0083109c* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor of C_0083109c: restores vtables, resets subsystem B8, frees the
// block at +0x1C and the 40-byte element array at +0x0C (each element's
// vtable is reset to the base class), then restores the base vtable.
static int __fastcall C_0083109c_Destroy(C_0083109c* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083109C);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00831094));
    C_0083109c_ResetSubsystemB8(self, nullptr);

    int result = static_cast<int>(self->field_001c);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    uint32_t arrayBase = self->field_000c;
    if (arrayBase) {
        uint8_t* data = reinterpret_cast<uint8_t*>(arrayBase);
        int elemCount = *reinterpret_cast<int*>(data - 4);
        for (int i = elemCount - 1; i >= 0; --i) {
            uint8_t* element = data + 40 * i;
            *reinterpret_cast<void***>(element + 4) = CSF_VTBL(0x0082D410);
        }
        result = CoreMemory_Free(data - 4);
        self->field_000c = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x00443F00, C_0083109c_Destroy, "auto");
