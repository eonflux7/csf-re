#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_DecodeColor, 0x00476430, (float* self, void* , unsigned int color));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, C_00860a30_SetFlagAndOrBits, 0x006C9FB0, (C_00860a30* self, void*));

static int __fastcall C_00862390_InitializeLight(C_00862390* self, void* /*edx*/)
{
    C_00860a30_SetFlagAndOrBits(
        reinterpret_cast<C_00860a30*>(self), nullptr);

    const int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(ResolveClumpManager(managerId, nullptr));
    CLightObject* light =
        reinterpret_cast<CLightObject*(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, manager, 0)[0x28 / 4])(manager, nullptr, 2, 3);

    auto* definition = reinterpret_cast<std::uint8_t*>(self->field_0018);
    alignas(8) std::uint8_t colorBytes[0x10];
    CSF_AT(float, colorBytes, 0x0) = 0.0f;
    CSF_AT(float, colorBytes, 0x4) = 0.0f;
    CSF_AT(float, colorBytes, 0x8) = 0.0f;
    CSF_AT(float, colorBytes, 0xC) = 1.0f;

    self->field_0020 = reinterpret_cast<std::uint32_t>(light);

    const unsigned int alpha = CSF_AT(std::uint8_t, definition, 0x4F);
    const unsigned int green = CSF_AT(std::uint8_t, definition, 0x4D);
    const unsigned int red = CSF_AT(std::uint8_t, definition, 0x4C);
    const unsigned int blue = CSF_AT(std::uint8_t, definition, 0x4E);
    const unsigned int packedColor =
        (alpha << 24) | (red << 16) | (green << 8) | blue;

    WeaponFirearms_DecodeColor(
        reinterpret_cast<float*>(colorBytes), nullptr, packedColor);
    light->vSlot34(reinterpret_cast<std::uint32_t*>(colorBytes));

    definition = reinterpret_cast<std::uint8_t*>(self->field_0018);
    light = reinterpret_cast<CLightObject*>(self->field_0020);
    light->vSlot3C(CSF_AT(int, definition, 0x50));

    reinterpret_cast<CLightObject*>(self->field_0020)->vSetValue3d(0);

    definition = reinterpret_cast<std::uint8_t*>(self->field_0018);
    if (CSF_AT(std::uint8_t, definition, 0x60) == 0)
        return reinterpret_cast<int>(definition);

    auto* auxiliary = static_cast<C_0086243c*>(Mem_Alloc(44));
    if (!auxiliary)
    {
        self->field_0034 = 0;
        return 0;
    }

    CSF_AT(std::uint32_t, auxiliary, 0x4) = 1;
    auxiliary->vtbl = CSF_VTBL(0x0086243C);
    auxiliary->field_000c = 0;
    auxiliary->field_0010 = 0;
    CSF_AT(std::uint32_t, auxiliary, 0x14) = 0;
    auxiliary->field_001c = 0;
    auxiliary->field_0020 = 0;
    CSF_AT(std::uint32_t, auxiliary, 0x24) = 0;
    auxiliary->field_0028 = 0;

    self->field_0034 = reinterpret_cast<std::uint32_t>(auxiliary);
    return reinterpret_cast<int>(auxiliary);
}

HOOK(0x006F1770, C_00862390_InitializeLight, "auto");
