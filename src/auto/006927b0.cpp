#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

// Copies at most (maxLen - 1) bytes from the string buffer into dst and NUL-terminates it.
// Returns the number of bytes copied.
static unsigned int __fastcall CoreParser_CopyBoundedString(void* self, void* /*edx*/, void* dst, int maxLen) {
    uint32_t storedLength = CSF_AT(uint32_t, self, 0x0);
    uint32_t maxCopy = static_cast<uint32_t>(maxLen) - 1;

    uint32_t copyLength = storedLength;
    if (storedLength > maxCopy) {
        copyLength = maxCopy;
    }

    std::memcpy(dst, CSF_AT(const uint8_t*, self, 0x4), copyLength);
    static_cast<uint8_t*>(dst)[copyLength] = 0;
    return copyLength;
}

HOOK(0x006927B0, CoreParser_CopyBoundedString, "auto");
