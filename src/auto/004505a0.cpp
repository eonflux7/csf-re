#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, sub_48E9D0, 0x0048E9D0, (void* self, void* edx, int a2));

// Walks the pointer array at +0x20 (count at +0x28) and returns the first entry that is
// enabled (flag bit 0 at +0x170), whose virtual slot 31 (+0x7C) reports false, and for which
// sub_48E9D0 accepts a2. Returns nullptr if none matches.
static uint8_t* __fastcall Container_FindFirstMatchingEnabledEntry(void* self, void* /*edx*/, int a2)
{
    uint8_t* result = nullptr;
    uint32_t count = CSF_AT(uint32_t, self, 0x28);

    for (uint32_t index = 0; index < count; ++index)
    {
        if (result)
            break;

        uint32_t entryAddress = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x20))[index];
        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);

        if (entry && (CSF_AT(uint8_t, entry, 0x170) & 1) != 0)
        {
            bool stateQuery = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[31])(entry, nullptr);
            if (!stateQuery)
            {
                if (sub_48E9D0(entry, nullptr, a2))
                    result = entry;
            }
        }
    }

    return result;
}

HOOK(0x004505A0, Container_FindFirstMatchingEnabledEntry, "auto");
