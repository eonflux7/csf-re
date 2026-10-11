#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, sub_7E1380, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

// Sets or clears bit 0x4 of the flags dword at +0x34 of the object held in field_0004,
// then forwards the new state to 0x7E1380 when the dword at +0x30 is non-null.
static int __fastcall C_00863dd0_SetEnabledFlag(C_00863dd0* self, void* /*edx*/, char a2) {
    uint32_t flags = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x34);
    if (a2) {
        flags = flags | 4u;
    } else {
        flags = flags & 0xFFFFFFFBu;
    }
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x34) = flags;

    uint32_t handle = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x30);
    if (handle != 0) {
        return reinterpret_cast<int>(sub_7E1380(reinterpret_cast<uint32_t*>(handle), 8, 1,
                                                static_cast<int>(a2 != 0)));
    }
    return static_cast<int>(handle);
}

HOOK(0x007122D0, C_00863dd0_SetEnabledFlag, "auto");
