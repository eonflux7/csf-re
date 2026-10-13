#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x69C950, called with `this` before the field read.
CSF_FUNC(int, __fastcall, sub_69C950, 0x0069C950, (void* self, void* edx));

static int __fastcall C_0085e92c_FlushStateAndGetField38(C_0085e92c* self, void* /*edx*/) {
    sub_69C950(self, nullptr);
    return static_cast<int>(self->field_0038);
}

HOOK(0x0069CBC0, C_0085e92c_FlushStateAndGetField38, "auto");
