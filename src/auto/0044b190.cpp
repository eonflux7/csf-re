#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x44B100: __thiscall, two stack args, ret 8.
CSF_FUNC(uint32_t*, __fastcall, SubTable_ApplyMatchedEntry, 0x0044B100, (void* self, void* edx, uint32_t* a2, int a3));

// Handles a type-8 message by searching the 12-byte entry table of this object
// for the key stored in message+12, then forwarding the matching entry to the
// sub-object at this+4.
static void __fastcall C_008311a0_HandleEntryMessage(C_008311a0* self, void* /*edx*/, void* message)
{
    if (CSF_AT(int32_t, message, 4) != 8)
        return;

    uint32_t entryBegin = self->field_0008;
    uint32_t entryCount = 0;
    if (entryBegin != 0)
    {
        int32_t byteSpan = CSF_AT(int32_t, self, 0x0C) - static_cast<int32_t>(entryBegin);
        entryCount = static_cast<uint32_t>(byteSpan / 12);
    }

    if (entryCount == 0)
        return;

    uint32_t searchValue = CSF_AT(uint32_t, message, 12);
    uint8_t* entryBase = reinterpret_cast<uint8_t*>(entryBegin);

    uint32_t index = 0;
    for (uint32_t* key = reinterpret_cast<uint32_t*>(entryBase + 4); searchValue != *key; key += 3)
    {
        if (++index >= entryCount)
            return;
    }

    uint32_t* matchedEntry = reinterpret_cast<uint32_t*>(entryBase + 12 * index);
    SubTable_ApplyMatchedEntry(reinterpret_cast<uint8_t*>(self) + 4, nullptr, &searchValue, reinterpret_cast<int>(matchedEntry));
}

HOOK(0x0044B190, C_008311a0_HandleEntryMessage, "auto");
