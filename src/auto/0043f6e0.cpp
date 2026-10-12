#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void**, __fastcall, Campaign_LookupEntry, 0x0043F2A0, (void* self, void* edx, void** a2, unsigned int* a3));

// Returns true when the entry found for `a2` in the table at self+4 does not
// hold the same dword as self+8.
static bool __fastcall Campaign_IsLookupMismatch(void* self, void* /*edx*/, int* a2)
{
    void** entry = Campaign_LookupEntry(
        static_cast<uint8_t*>(self) + 4,
        nullptr,
        reinterpret_cast<void**>(&a2),
        reinterpret_cast<unsigned int*>(a2));

    uint32_t entryValue = reinterpret_cast<uint32_t>(*entry);
    uint32_t storedValue = CSF_AT(uint32_t, self, 8);
    return entryValue != storedValue;
}

HOOK(0x0043F6E0, Campaign_IsLookupMismatch, "auto");
