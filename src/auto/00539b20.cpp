#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (std::uint32_t* self, void*));
CSF_FUNC(int, __cdecl, ProfilesSavegames_ReleaseSlotBuffer, 0x005129A0, (int firstSlot, int endSlot));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall CMenuLoadGame_DestroyResources(
    CMenuLoadGame* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0014);

    self->vtbl = CSF_VTBL(0x00842220);
    self->field_0004 =
        reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00842280));

    if (child)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
        self->field_0014 = 0;
    }

    const std::uint32_t firstSlot = self->field_010c;
    if (firstSlot)
    {
        ProfilesSavegames_ReleaseSlotBuffer(
            static_cast<int>(firstSlot),
            static_cast<int>(CSF_AT(std::uint32_t, self, 0x110)));

        if (self->field_010c)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_010c));
    }

    self->field_010c = 0;
    CSF_AT(std::uint32_t, self, 0x110) = 0;
    CSF_AT(std::uint32_t, self, 0x114) = 0;

    unsigned int result = SaveGame_DestroyRecord(
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x38),
        nullptr);

    if (self->field_0034 >= 0x10)
    {
        result = self->field_0020;
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0020));
    }

    self->field_0030 = 0;
    self->field_0034 = 15;
    *reinterpret_cast<std::uint8_t*>(&self->field_0020) = 0;

    self->field_0004 =
        reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00830488));
    self->field_0004 =
        reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00830438));
    self->vtbl = CSF_VTBL(0x0083042C);

    return result;
}

HOOK(0x00539B20, CMenuLoadGame_DestroyResources, "auto");
