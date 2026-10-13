#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Stack-popping thiscall (ret 12) string assignment from the original binary.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the 44-byte records in [first, last) from the template record at `source`.
// Each entry gets its string reassigned from the template, then four dwords copied.
// Returns the last template dword copied (0 if the range is empty).
static uint32_t __cdecl RtFs_CopyFileSystemEntries(uint8_t* first, uint8_t* last, uint8_t* source)
{
    uint32_t result = 0;
    for (uint8_t* entry = first; entry != last; entry += 44)
    {
        AssignStringSubstring(entry, nullptr, source, 0u, 0xFFFFFFFFu);
        CSF_AT(uint32_t, entry, 0x1C) = CSF_AT(uint32_t, source, 0x1C);
        CSF_AT(uint32_t, entry, 0x20) = CSF_AT(uint32_t, source, 0x20);
        CSF_AT(uint32_t, entry, 0x24) = CSF_AT(uint32_t, source, 0x24);
        result = CSF_AT(uint32_t, source, 0x28);
        CSF_AT(uint32_t, entry, 0x28) = result;
    }
    return result;
}

HOOK(0x0069ECC0, RtFs_CopyFileSystemEntries, "auto");
