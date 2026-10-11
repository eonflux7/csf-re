#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxSkin_SetPropertyValue, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));

static int __stdcall GfxSkin_InitializeDefaultProperties(C_008670f0* self)
{
    GfxSkin_SetPropertyValue(self, nullptr, 3, 1065353216);
    GfxSkin_SetPropertyValue(self, nullptr, 5, 0);

    // The original replaces only the low byte of its pointer argument.
    const std::uint32_t pointerHighBytes =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) & ~0xFFu;

    GfxSkin_SetPropertyValue(self, nullptr, 0, static_cast<int>(pointerHighBytes | 1u));
    GfxSkin_SetPropertyValue(self, nullptr, 1, 1);
    GfxSkin_SetPropertyValue(self, nullptr, 2, 0);
    GfxSkin_SetPropertyValue(self, nullptr, 4, static_cast<int>(pointerHighBytes));
    GfxSkin_SetPropertyValue(self, nullptr, 7, 0);
    GfxSkin_SetPropertyValue(self, nullptr, 6, 0);

    Subobject_SetKeyedValue(self, nullptr, 1, 0);
    Subobject_SetKeyedValue(self, nullptr, 6, 1u);
    Subobject_SetKeyedValue(self, nullptr, 8, 1u);
    Subobject_SetKeyedValue(self, nullptr, 7, 2u);
    Subobject_SetKeyedValue(self, nullptr, 20, 2u);
    Subobject_SetKeyedValue(self, nullptr, 10, 2u);
    Subobject_SetKeyedValue(self, nullptr, 11, 1u);
    Subobject_SetKeyedValue(self, nullptr, 12, 0);
    Subobject_SetKeyedValue(self, nullptr, 29, 8u);
    Subobject_SetKeyedValue(self, nullptr, 30, 0);
    Subobject_SetKeyedValue(self, nullptr, 14, 0);
    Subobject_SetKeyedValue(self, nullptr, 15, 0);
    Subobject_SetKeyedValue(self, nullptr, 16, 1u);
    return Subobject_SetKeyedValue(
        self, nullptr, 17,
        static_cast<unsigned int>(
            reinterpret_cast<std::uintptr_t>(CSF_ADDR_DATA(0x008BB4C4))));
}

HOOK(0x006C7C80, GfxSkin_InitializeDefaultProperties, "auto");
