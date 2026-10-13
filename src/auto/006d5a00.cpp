#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GfxScene_UpdateAuxiliaryObject(void* self, void* /*edx*/, int a2)
{
    uint8_t enabled = CSF_AT(uint8_t, self, 0x01);
    if (enabled == 0)
        return 0;

    uint32_t count = CSF_AT(uint32_t, self, 0x0C);
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t arrayBase = CSF_AT(uint32_t, self, 0x04);
        uint32_t child = CSF_AT(uint32_t, reinterpret_cast<void*>(arrayBase), 4 * i);
        if (child != 0)
        {
            void* obj = reinterpret_cast<void*>(child);
            // vtable slot at +0x14 -> index 5
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[5])(obj, nullptr, a2);
        }
        count = CSF_AT(uint32_t, self, 0x0C);
    }
    return static_cast<int>(count);
}

HOOK(0x006D5A00, GfxScene_UpdateAuxiliaryObject, "auto");
