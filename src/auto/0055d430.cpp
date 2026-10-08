#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Helper bound by address: __cdecl (dst, src) copier, returns eax as-is.
CSF_FUNC(int, __cdecl, MenuAccounts_LegacyCopyWide, 0x00695680, (char* dst, char* src));

// Helper bound by address: __cdecl wide-string length in WORDs, returns eax as-is.
CSF_FUNC(int, __cdecl, MenuAccounts_LegacyWideLength, 0x00695560, (uint16_t* str));

// Copies a wide name into the caller buffer. When flag 0x40 (at +0x200) is set,
// the source is the embedded buffer at +2, and the buffer's last WORD (index len-1)
// is zeroed, where len is the returned WORD count.
static int __fastcall MenuAccounts_CopyWideNameToBuffer(uint8_t* self, void* /*edx*/, char* outBuffer)
{
    if ((CSF_AT(uint8_t, self, 0x200) & 0x40) == 0)
    {
        return MenuAccounts_LegacyCopyWide(outBuffer, reinterpret_cast<char*>(self));
    }

    MenuAccounts_LegacyCopyWide(outBuffer, reinterpret_cast<char*>(self + 2));
    int wideLength = MenuAccounts_LegacyWideLength(reinterpret_cast<uint16_t*>(outBuffer));
    reinterpret_cast<uint16_t*>(outBuffer)[wideLength - 1] = 0;
    return wideLength;
}

HOOK(0x0055D430, MenuAccounts_CopyWideNameToBuffer, "auto");
