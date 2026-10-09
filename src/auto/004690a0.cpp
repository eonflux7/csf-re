#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneManager_UnbindObjectFromId, 0x00468CC0, (void* self, void* edx, int a2, int a3));

// Clears every slot equal to a2 in each 16-byte record of the table at +0x68 (record: ptr at +0, count at +8),
// then unbinds a2 from the scene for each record index.
static void __fastcall S_0045d610_ClearIdEntries(S_0045d610* self, void* /*edx*/, int a2)
{
    if (a2 == 0)
    {
        return;
    }

    uint32_t recordCount = 0;
    if (self->field_0068 != 0)
    {
        int32_t tableSpan = static_cast<int32_t>(self->field_006c - self->field_0068);
        recordCount = static_cast<uint32_t>(tableSpan >> 4);
    }

    uint32_t recordByteOffset = 0;
    for (uint32_t recordIndex = 0; recordIndex < recordCount; ++recordIndex)
    {
        void* record = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0068 + recordByteOffset));
        uint32_t slotCount = CSF_AT(uint32_t, record, 0x8);

        for (uint32_t slot = 0; slot < slotCount; ++slot)
        {
            void* currentRecord = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0068 + recordByteOffset));
            uint32_t* slots = CSF_AT(uint32_t*, currentRecord, 0);
            if (slots[slot] == static_cast<uint32_t>(a2))
            {
                slots[slot] = 0;
            }
        }

        SceneManager_UnbindObjectFromId(self, nullptr, a2, static_cast<int>(recordIndex));
        recordByteOffset += 16;
    }
}

HOOK(0x004690A0, S_0045d610_ClearIdEntries, "auto");
