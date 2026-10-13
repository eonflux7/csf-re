#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CharTask_LookupByOffset(this, edx, a2): ret 4
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Returns the task found by looking up (this->field_000c + 0xC4) through CharTask_LookupByOffset.
static int __fastcall C_00834fa8_LookupTaskAtFieldOffset(C_00834fa8* self, void* /*edx*/)
{
    const int lookupOffset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0xC4);
    return CharTask_LookupByOffset(self, nullptr, lookupOffset);
}

HOOK(0x004A7B90, C_00834fa8_LookupTaskAtFieldOffset, "auto");
