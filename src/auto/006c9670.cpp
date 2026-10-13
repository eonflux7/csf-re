#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340, (int* first, int* last));
CSF_FUNC(int, __cdecl, MenuSaveGame_MoveSlotRange, 0x006C8FE0, (int a1, int a2, int a3));

static int __fastcall C_00860954_RemoveInactiveSlots(
    C_00860954* self, void* /*edx*/)
{
    int count = 0;
    if (self->field_0008)
    {
        count = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x0C) - self->field_0008) / 36;
    }

    int result = count - 1;
    if (result >= 0)
    {
        int offset = 36 * result;
        int remaining = result + 1;
        do
        {
            const uint32_t slotAddress = self->field_0008 + offset;
            auto* slot = reinterpret_cast<uint8_t*>(slotAddress);

            if (!CSF_AT(uint32_t, slot, 0x1C) &&
                !CSF_AT(uint32_t, slot, 0x14))
            {
                MenuSaveGame_MoveSlotRange(
                    static_cast<int>(slotAddress + 36),
                    static_cast<int>(CSF_AT(uint32_t, self, 0x0C)),
                    static_cast<int>(slotAddress));

                const uint32_t end = CSF_AT(uint32_t, self, 0x0C);
                MenuSaveGame_ClearSlotRange(
                    reinterpret_cast<int*>(end - 36),
                    reinterpret_cast<int*>(end));

                CSF_AT(uint32_t, self, 0x0C) -= 36;
            }

            offset -= 36;
            result = --remaining;
        } while (remaining);
    }

    return result;
}

HOOK(0x006C9670, C_00860954_RemoveInactiveSlots, "auto");
