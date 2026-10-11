#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

// Walks the entry array of `self` (8-byte records, name pointer at entry+0x14) and returns the
// index of the first entry whose name equals the string described by `a2`, or -1 if none matches.
// `a2` is a string descriptor: length at +0x14, inline buffer at +4 (when length < 0x10), or
// a heap pointer stored at +4 (when length >= 0x10); length at +0x18 selects which.
static int __fastcall S_006b9580_FindMatchingEntryIndex(S_006b9580* self, void* /*edx*/, void* a2)
{
    uint32_t i = 0;
    for (;;)
    {
        uint32_t entries = self->field_0004;
        if (entries == 0 || i >= static_cast<uint32_t>(static_cast<int32_t>(self->field_0008 - entries) >> 3))
            break;

        void* entry = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entries), 8 * i));

        void* otherText;
        if (CSF_AT(uint32_t, a2, 0x18) < 0x10u)
            otherText = static_cast<uint8_t*>(a2) + 4;
        else
            otherText = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));

        if (!CompareStringSubstring(entry, nullptr, 0u,
                                    CSF_AT(uint32_t, entry, 0x14),
                                    static_cast<const char*>(otherText),
                                    CSF_AT(uint32_t, a2, 0x14)))
        {
            return static_cast<int>(i);
        }
        ++i;
    }
    return -1;
}

HOOK(0x6B9580, S_006b9580_FindMatchingEntryIndex, "auto");
