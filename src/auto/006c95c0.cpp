#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, SaveGame_DestroySlotData, 0x006C2CF0, (void* self, void* edx));

static int __fastcall C_00860954_ClearSlots(C_00860954* self, void* /*edx*/)
{
    uint32_t index = 0;
    uint32_t byteOffset = 0;

    for (;;)
    {
        const uint32_t first = self->field_0008;
        if (!first)
            break;

        const int32_t byteCount =
            static_cast<int32_t>(self->field_000c - first);
        if (index >= static_cast<uint32_t>(byteCount / 36))
            break;

        void* slotData = CSF_AT(
            void*,
            reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset,
            0x1C);

        if (slotData)
        {
            SaveGame_DestroySlotData(slotData, nullptr);
            Mem_Free(slotData);
        }

        ++index;
        byteOffset += 36;
    }

    if (self->field_0008)
    {
        MenuSaveGame_ClearSlotRange(
            reinterpret_cast<int*>(self->field_0008),
            reinterpret_cast<int*>(self->field_000c));

        if (self->field_0008)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return 0;
}

HOOK(0x006C95C0, C_00860954_ClearSlots, "auto");
