#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00860a30_Initialize, 0x006CA660, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_00861bd8_InitWithSkinFlags, 0x006E5210, (C_00861bd8* self, void* , int** a2, char a3));

static std::uint32_t* __fastcall C_00862490_Initialize(
    C_00862490* self, void* /*edx*/)
{
    C_00860a30_Initialize(self, nullptr);

    self->vtbl = CSF_VTBL(0x00862490);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00862488));
    self->field_0008 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x0086246C));
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_0034 = 0.0f;
    self->field_0048 = 0;
    self->field_0044 = -1;
    self->field_0054 = -1;
    self->field_0084 = 0;
    self->field_0050 = 0;
    self->field_0058 = static_cast<std::uint8_t>(-1);
    self->field_0088 = 0;
    self->field_008c = 10;
    self->field_0024 = 0;
    self->field_0038 = 1.0f;
    self->field_003c = 1.0f;
    self->field_0028 = 1;
    self->field_0020 = 0;
    self->field_004c = 0;

    auto* skinFlags = static_cast<C_00861bd8*>(Mem_Alloc(20));
    if (skinFlags)
    {
        void* skinOwner = reinterpret_cast<void*>(self->field_001c);
        int** skin = reinterpret_cast<int** (__fastcall*)(void*, void*)>(
            CSF_AT(void**, skinOwner, 0)[0x1C / 4])(skinOwner, nullptr);

        self->field_0040 = reinterpret_cast<std::uint32_t>(
            C_00861bd8_InitWithSkinFlags(skinFlags, nullptr, skin, 0));
    }
    else
    {
        self->field_0040 = 0;
    }

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x006F1A00, C_00862490_Initialize, "auto");
