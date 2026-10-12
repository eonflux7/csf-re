#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, HudStringShow_PrepareEntry, 0x004F33D0,
         (void* self, void* edx, void* a1, unsigned int a3, int a2));

static uint32_t* __fastcall S_004f36f0_StoreEntryPointerForItem(
    S_004f36f0* self, void* /*edx*/, uint32_t* output,
    C_0082d410* item, int context)
{
    int32_t entryIndex = 0;
    uint32_t firstEntry = self->field_0004;
    if (firstEntry && static_cast<int32_t>(self->field_0008 - firstEntry) / 124)
    {
        entryIndex = static_cast<int32_t>(
            reinterpret_cast<uint32_t>(item) - firstEntry) / 124;
    }

    HudStringShow_PrepareEntry(self, nullptr, item, 1u, context);

    *output = self->field_0004 + 124u * static_cast<uint32_t>(entryIndex);
    return output;
}

HOOK(0x004F36F0, S_004f36f0_StoreEntryPointerForItem, "auto");
