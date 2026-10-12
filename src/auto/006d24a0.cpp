#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, sub_6D2440, 0x006D2440, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6DAD40, 0x006DAD40, (void* self, void* edx));

// Releases every heap buffer owned by the light-world entry and resets its pointers.
// Returns the low byte of the last Mem_Free result, or 0 if the last pointer was null.
static char __fastcall S_006d24a0_Release(S_006d24a0* self, void* /*edx*/) {
    sub_6DAD40(self, nullptr);

    if (self->field_00f0) {
        Mem_Free(reinterpret_cast<void*>(self->field_00f0));
        self->field_00f0 = 0;
    }
    if (self->field_00d8) {
        Mem_Free(reinterpret_cast<void*>(self->field_00d8));
        self->field_00d8 = 0;
    }

    sub_6D2440(reinterpret_cast<uint8_t*>(self) + 0x84, nullptr);
    sub_6D2440(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    if (self->field_0024) {
        Mem_Free(reinterpret_cast<void*>(self->field_0024));
        self->field_0024 = 0;
    }

    char result = 0;
    if (self->field_000c) {
        result = static_cast<char>(Mem_Free(reinterpret_cast<void*>(self->field_000c)));
        self->field_000c = 0;
    }
    return result;
}

HOOK(0x6D24A0, S_006d24a0_Release, "auto");
