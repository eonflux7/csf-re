#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x49CFA0: __thiscall with one stack argument (ret 4), bound as __fastcall.
CSF_FUNC(int, __fastcall, SceneLoader_ApplyFocusTarget, 0x0049CFA0, (void* self, void* edx, uint32_t* a2));

// Original: char __thiscall (S_0049cfa0* this, int a2, int a3), ret 8.
static char __fastcall SceneLoader_HandleFocusRequest(S_0049cfa0* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* request = reinterpret_cast<uint8_t*>(a2);

    uint8_t result = self->field_0184;
    if (result != 0)
        return static_cast<char>(result);

    if (CSF_AT(uint32_t, request, 0x4C) != 0)
    {
        if (self->field_0050 == 16)
            SceneLoader_ApplyFocusTarget(self, nullptr, reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, request, 0x4C)));
    }

    // Copy three dwords from request+0x2C into self+0x19C..0x1A4 (the pad_0185 area).
    uint8_t* source = request + 0x2C;
    CSF_AT(uint32_t, self, 0x19C) = CSF_AT(uint32_t, source, 0x00);
    CSF_AT(uint32_t, self, 0x1A0) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0x1A4) = CSF_AT(uint32_t, source, 0x08);

    // Virtual call: vtable (first dword of self) slot 0xA8 / 4 = 42, with a3 as the argument.
    void** vtbl = reinterpret_cast<void**>(self->field_0000);
    using FocusVirtualFn = char(__fastcall*)(void* self, void* edx, int a3);
    return reinterpret_cast<FocusVirtualFn>(vtbl[0xA8 / 4])(self, nullptr, a3);
}

HOOK(0x0049D220, SceneLoader_HandleFocusRequest, "auto");
