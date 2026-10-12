#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

static void MpGameModes_DestroyAndClearObject(uint32_t& reference)
{
    void* object = reinterpret_cast<void*>(reference);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        reference = 0;
    }
}

static int __fastcall C_008445f4_DestroyOwnedObjects(
    C_008445f4* self, void* /*edx*/)
{
    uint8_t* entries = reinterpret_cast<uint8_t*>(self) + 0x7E4;
    for (int row = 0; row < 100; ++row)
    {
        for (int column = 0; column < 5; ++column)
        {
            MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries, 0));
            entries += 4;
        }
    }

    entries = reinterpret_cast<uint8_t*>(self) + 0x1064;
    for (int index = 0; index < 5; ++index)
    {
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries - 0x28, 0));
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries, 0));
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries + 0x14, 0));
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries - 0x14, 0));
        entries += 4;
    }

    entries = reinterpret_cast<uint8_t*>(self) + 0xFD0;
    for (int index = 0; index < 5; ++index)
    {
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries - 0x14, 0));
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries, 0));
        entries += 4;
    }

    entries = reinterpret_cast<uint8_t*>(self) + 0x1010;
    for (int index = 0; index < 11; ++index)
    {
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries - 0x2C, 0));
        MpGameModes_DestroyAndClearObject(CSF_AT(uint32_t, entries, 0));
        entries += 4;
    }

    MpGameModes_DestroyAndClearObject(self->field_0fb4);
    return ReleaseObjectReference(
        reinterpret_cast<void*>(self->field_0fb8), nullptr);
}

HOOK(0x0054ED50, C_008445f4_DestroyOwnedObjects, "auto");
