#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"


CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0,
         (void* self, void* edx, int additionalCapacity));
CSF_FUNC(std::uint32_t*, __fastcall, InitializeDefaultColorPalette, 0x0071DF80,
         (std::uint32_t* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, CreateVariantObject, 0x0072CB10,
         (int variant));

static CGuiBuilder* __fastcall CGuiBuilder_Construct(
    CGuiBuilder* self, void* /*edx*/, int owner, char flag, int variant)
{
    CSF_AT(std::uint32_t, self, 0x00) =
        static_cast<std::uint32_t>(CSF_ADDR_VT(0x00864B30));
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;

    auto* controlArray = reinterpret_cast<std::uint8_t*>(self) + 0x0C;

    self->field_002c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0020 = 0;
    self->field_0030 = 5;
    self->field_0034 = 0x3FC00000;
    self->field_0038 = 0;
    self->field_003c = 0x3F000000;

    InitializeDefaultColorPalette(
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x40),
        nullptr);

    self->field_0004 = static_cast<std::uint32_t>(owner);
    self->field_0008 = static_cast<std::uint8_t>(flag);
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0014 = 0;

    const std::uint32_t capacity = self->field_0010;
    if (capacity < 2)
        GrowButtonArray(controlArray, nullptr, static_cast<int>(2 - capacity));

    self->field_0014 = 2;
    const std::uint32_t count = self->field_0014;
    for (std::uint32_t index = 0; index < count; ++index)
        reinterpret_cast<std::uint32_t*>(self->field_000c)[index] = 0;

    self->field_001c =
        reinterpret_cast<std::uint32_t>(CreateVariantObject(variant));
    return self;
}

HOOK(0x0071F3F0, CGuiBuilder_Construct, "auto");
