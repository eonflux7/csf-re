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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Resets a string-like record at `self` to empty and assigns the full contents of `source` into it.
// Returns `self`, or nullptr when `self` is null.
static C_0082d410* __stdcall C_0082d410_CopyConstructFrom(C_0082d410* self, C_0082d410* source)
{
    C_0082d410* result = nullptr;
    if (self)
    {
        CSF_AT(std::uint32_t, self, 0x14) = 0;    // length = 0
        CSF_AT(std::uint32_t, self, 0x18) = 15;   // capacity = 15 (small-buffer size)
        CSF_AT(std::uint8_t, self, 0x04) = 0;     // first byte of inline buffer = '\0'
        result = reinterpret_cast<C_0082d410*>(
            AssignStringSubstring(self, nullptr, source, 0, 0xFFFFFFFFu));
    }
    return result;
}

HOOK(0x00410B50, C_0082d410_CopyConstructFrom, "auto");
