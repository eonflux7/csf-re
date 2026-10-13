#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TCharTask_ForwardUpdateToChild, 0x004DB0F0, (void* self, void* edx, int a2));

// Lazily resolves the child task pointer at +0x10 (via the table lookup at +0x0C + 20),
// then forwards the update request to the child task.
static int __fastcall C_00834fa8_ResolveChildAndForwardUpdate(C_00834fa8* self, void* /*edx*/, int a2)
{
    if (CSF_AT(uint32_t, self, 0x10) == 0) {
        CharTask_LookupByOffset(self, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 20));
    }
    return TCharTask_ForwardUpdateToChild(self, nullptr, a2);
}

HOOK(0x004A6F40, C_00834fa8_ResolveChildAndForwardUpdate, "auto");
