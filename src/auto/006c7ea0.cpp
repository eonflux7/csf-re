#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedFlag, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(int, __stdcall, GfxSkin_ConfigureDefaultPreset, 0x006C7C80, (void* a1));
CSF_FUNC(char, __fastcall, GfxSkin_GrowAuxiliaryCapacity, 0x006C7DD0, (void* self, void* edx, int a2));

static void* C_008608f8_CreatePreset(C_008608f8* self, int index)
{
    void* memory = Mem_Alloc(20);
    if (!memory)
        return nullptr;

    int parent = reinterpret_cast<int*>(self->field_0004)[0];
    return reinterpret_cast<void*>(
        C_00860894_Init(memory, nullptr, index, parent));
}

static std::uint32_t* __fastcall C_008608f8_Construct(
    C_008608f8* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008608F8);

    self->field_0004 = 0;
    CSF_AT(std::uint32_t, self, 0x008) = 0;
    self->field_000c = 0;
    CSF_AT(std::uint32_t, self, 0x010) = 10;
    self->field_0014 = 0;
    CSF_AT(std::uint32_t, self, 0x018) = 0;
    CSF_AT(std::uint32_t, self, 0x01C) = 0;
    CSF_AT(std::uint32_t, self, 0x020) = 10;

    // The packet also identifies the direct grow-call address as a vtable.
    (void)CSF_VTBL(0x0041E5F0);
    std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x008);
    if (capacity < 0xE)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0004),
            nullptr, 14 - capacity);
    }

    for (std::uint32_t i = self->field_000c; i < 0xE; ++i)
        reinterpret_cast<std::uint32_t*>(self->field_0004)[i] = 0;
    self->field_000c = 14;

    void* memory = Mem_Alloc(20);
    void* preset = memory
        ? reinterpret_cast<void*>(C_00860894_Init(memory, nullptr, 0, 0))
        : nullptr;
    GfxSkin_ConfigureDefaultPreset(preset);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[0] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 1);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[1] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 2);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[2] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 3);
    Subobject_SetKeyedValue(preset, nullptr, 1, 0);
    GfxSkin_SetKeyedFlag(preset, nullptr, 4, 0);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[3] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 4);
    Subobject_SetKeyedValue(preset, nullptr, 14, 0);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[4] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 5);
    Subobject_SetKeyedValue(preset, nullptr, 14, 0);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[5] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 6);
    Subobject_SetKeyedValue(preset, nullptr, 29, 5u);
    Subobject_SetKeyedValue(preset, nullptr, 20, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 30, 0x20u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[6] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 7);
    Subobject_SetKeyedValue(preset, nullptr, 12, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 8, 0);
    Subobject_SetKeyedValue(preset, nullptr, 20, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 10, 5u);
    Subobject_SetKeyedValue(preset, nullptr, 11, 6u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[7] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 8);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[8] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 9);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[9] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 12);
    Subobject_SetKeyedValue(preset, nullptr, 12, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 1, 0);
    Subobject_SetKeyedValue(preset, nullptr, 20, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 10, 5u);
    Subobject_SetKeyedValue(preset, nullptr, 11, 6u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[12] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 13);
    Subobject_SetKeyedValue(preset, nullptr, 12, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 6, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 8, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 1, 0);
    Subobject_SetKeyedValue(preset, nullptr, 20, 2u);
    Subobject_SetKeyedValue(preset, nullptr, 10, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 11, 2u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[13] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 10);
    Subobject_SetKeyedValue(preset, nullptr, 12, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 8, 0);
    Subobject_SetKeyedValue(preset, nullptr, 6, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 20, 2u);
    GfxSkin_SetKeyedFlag(preset, nullptr, 4, 0);
    Subobject_SetKeyedValue(preset, nullptr, 14, 1u);
    Subobject_SetKeyedValue(preset, nullptr, 10, 5u);
    Subobject_SetKeyedValue(preset, nullptr, 11, 4u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[10] =
        reinterpret_cast<std::uint32_t>(preset);

    preset = C_008608f8_CreatePreset(self, 11);
    Subobject_SetKeyedValue(preset, nullptr, 12, 0);
    Subobject_SetKeyedValue(preset, nullptr, 8, 0);
    Subobject_SetKeyedValue(preset, nullptr, 6, 0);
    Subobject_SetKeyedValue(preset, nullptr, 14, 0);
    Subobject_SetKeyedValue(preset, nullptr, 10, 3u);
    Subobject_SetKeyedValue(preset, nullptr, 11, 9u);
    reinterpret_cast<std::uint32_t*>(self->field_0004)[11] =
        reinterpret_cast<std::uint32_t>(preset);

    capacity = CSF_AT(std::uint32_t, self, 0x018);
    if (capacity < 0xA)
        GfxSkin_GrowAuxiliaryCapacity(&self->field_0014, nullptr, 10 - capacity);

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x006C7EA0, C_008608f8_Construct, "auto");
