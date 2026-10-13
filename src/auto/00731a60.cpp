#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// Original: __thiscall, 2 stack args (ret 8). Ported as __fastcall with the second stack arg in edx's slot,
// i.e. (self, edx, a2, a3), which pops 8 bytes on return.
static int __fastcall C_00867078_ConfigureEffect(C_00867078* self, void* /*edx*/, int effect, int argument)
{
    int result = CanConfigureEffect(self, nullptr, effect);
    if (result) {
        void* target = reinterpret_cast<void*>(result);
        // vtable slot 1 (byte offset +0x4) of the object returned by CanConfigureEffect
        auto slot = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, target, 0)[1]);
        return slot(target, nullptr, argument);
    }
    return result;
}

HOOK(0x731A60, C_00867078_ConfigureEffect, "auto");
