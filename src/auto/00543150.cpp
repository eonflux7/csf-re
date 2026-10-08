#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, C_00830438_Construct,
         0x00712CC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());

static void** const g_MenuScreenVtable =
    CSF_VTBL(0x0083042C);
static void** const g_MenuControlsEmbeddedVtable =
    reinterpret_cast<void**>(0x008435A8);
static void** const g_MenuControlsVtable =
    CSF_VTBL(0x008435F8);

static CMenuControls* __fastcall CMenuControls_Construct(
    CMenuControls* self, void* /*edx*/, int configuration)
{
    CSF_AT(void**, self, 0) = g_MenuScreenVtable;

    C_00830438_Construct(
        reinterpret_cast<std::uint8_t*>(self) + 4, nullptr);

    self->field_0004 =
        reinterpret_cast<std::uintptr_t>(g_MenuControlsEmbeddedVtable);
    CSF_AT(void**, self, 0) = g_MenuControlsVtable;

    auto* entry = reinterpret_cast<std::uint8_t*>(self) + 0x48;
    for (int remaining = 32; remaining != 0; --remaining, entry += 0x14)
    {
        CSF_AT(std::uint32_t, entry, 0x00) = 0xFFFFFFFFu;
        CSF_AT(std::uint32_t, entry, 0x04) = 0xFFFFFFFFu;
        CSF_AT(std::uint32_t, entry, 0x08) = 0;
        CSF_AT(std::uint32_t, entry, 0x0C) = 0x3F800000u;
        CSF_AT(std::uint32_t, entry, 0x10) = 0;
    }

    self->field_0024 = static_cast<std::uint32_t>(configuration);
    self->field_0014 = 0;

    auto* globalState = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_8B056C()));
    const std::uint32_t owner = CSF_AT(std::uint32_t, globalState, 0x18);
    self->field_001c = owner;

    const std::uint32_t ownerData = owner
        ? CSF_AT(std::uint32_t, reinterpret_cast<void*>(owner), 0x04)
        : 0;

    void** vtable = CSF_AT(void**, self, 0);
    self->field_0020 = ownerData;
    self->field_0018 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_02d4 = 0xFFFFFFFFu;
    self->field_02d8 = 0xFFFFFFFFu;
    self->field_02cc = 0xFFFFFFFFu;
    self->field_02d0 = 0xFFFFFFFFu;
    self->field_02dc = 0xFFFFFFFFu;

    reinterpret_cast<void (__fastcall*)(void*, void*)>(vtable[0x40 / 4])(
        self, nullptr);
    return self;
}

HOOK(0x00543150, CMenuControls_Construct, "auto");
