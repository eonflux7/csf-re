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

// Returns 1 when the child task stored at +0x10 is absent or reports itself
// as not active (its vtable slot 32 returns 0); otherwise returns 0.
static int __fastcall C_0082f9f8_IsSubTaskNotActive(C_0082f9f8* self, void* /*edx*/) {
    uint32_t childAddress = CSF_AT(uint32_t, self, 0x10);
    if (childAddress == 0) {
        return 1;
    }

    void* child = reinterpret_cast<void*>(childAddress);
    int childReply = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, child, 0)[128 / 4])(child, nullptr);

    if (childReply == 0) {
        return 1;
    }
    return 0;
}

HOOK(0x0047B240, C_0082f9f8_IsSubTaskNotActive, "auto");
