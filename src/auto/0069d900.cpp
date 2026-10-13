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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the string member at +0x41C of `other` into the same member of `self`
// (position 0, count -1 = whole string). Returns `self`.
static void* __fastcall CErrorLog_CopyFileNameFrom(void* self, void* /*edx*/, const void* other)
{
    uint8_t* selfBytes = static_cast<uint8_t*>(self);
    const uint8_t* otherBytes = static_cast<const uint8_t*>(other);

    AssignStringSubstring(selfBytes + 0x41C, nullptr, otherBytes + 0x41C, 0u, 0xFFFFFFFFu);
    return self;
}

HOOK(0x0069D900, CErrorLog_CopyFileNameFrom, "auto");
