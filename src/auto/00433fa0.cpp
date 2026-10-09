#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004109E0, rets=12. Assigns a substring of `source` to `self`.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Assigns the whole source string (position 0, count -1 = until end) to this object.
// Original: push -1; push 0; push a2; call AssignStringSubstring; ret 4
static int __fastcall C_0082d410_AssignWholeString(C_0082d410* self, void* /*edx*/, int sourceValue)
{
    const void* source = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(sourceValue));
    void* result = AssignStringSubstring(self, nullptr, source, 0u, 0xFFFFFFFFu);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x433FA0, C_0082d410_AssignWholeString, "auto");
