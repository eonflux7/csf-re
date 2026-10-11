#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0, (void* self, void* edx, unsigned int offset, unsigned int length, const uint16_t* text, unsigned int textLength));

// __thiscall with one stack argument (entry), returns via `ret 4`.
// Entries with a stored name length below 8 keep the text inline at +4;
// longer names are reached through the pointer stored at +4.
static int __fastcall ProfileName_CompareInlineOrHeap(void* self, void* /*edx*/, void* entry)
{
    if (CSF_AT(uint32_t, entry, 0x18) < 8u)
    {
        const uint32_t textLength = CSF_AT(uint32_t, entry, 0x14);
        const uint16_t* inlineText = reinterpret_cast<const uint16_t*>(static_cast<uint8_t*>(entry) + 4);
        // edx still holds entry[+0x14] at the call site in the original.
        return MenuMpAccounts_CompareNames(self,
            reinterpret_cast<void*>(static_cast<uintptr_t>(textLength)),
            0u,
            CSF_AT(uint32_t, self, 0x14),
            inlineText,
            textLength);
    }
    else
    {
        const uint32_t textLength = CSF_AT(uint32_t, entry, 0x14);
        const uint16_t* heapText = reinterpret_cast<const uint16_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 0x4)));
        return MenuMpAccounts_CompareNames(self,
            reinterpret_cast<void*>(static_cast<uintptr_t>(textLength)),
            0u,
            CSF_AT(uint32_t, self, 0x14),
            heapText,
            textLength);
    }
}

HOOK(0x00515AB0, ProfileName_CompareInlineOrHeap, "auto");
