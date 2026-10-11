#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_6C63D0, 0x006C63D0, (void* self, void* edx, int a2));

// __thiscall char C_008656d0::sub_6C68B0(int a2), ret 4.
// Increments the per-object counter at +0xFC. If flag bit 8 (field_0008) is set,
// forwards a2->[+0x1C] to sub_6C63D0 and, on success, increments the counter at +0x10C.
static char __fastcall C_008656d0_ProcessWithFlag8(C_008656d0* self, void* /*edx*/, int a2)
{
    uint32_t visitCounter = CSF_AT(uint32_t, self, 0xFC);
    uint8_t flags = static_cast<uint8_t>(self->field_0008);
    char result = 0;

    CSF_AT(uint32_t, self, 0xFC) = visitCounter + 1;

    if ((flags & 8) != 0) {
        int subobjectRef = CSF_AT(int, reinterpret_cast<void*>(a2), 0x1C);
        result = sub_6C63D0(self, nullptr, subobjectRef);
        if (result) {
            ++CSF_AT(uint32_t, self, 0x10C);
        }
    }
    return result;
}

HOOK(0x006C68B0, C_008656d0_ProcessWithFlag8, "auto");
