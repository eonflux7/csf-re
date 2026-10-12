#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a 0x94-byte rules block into the object at +0x18, then invokes the
// object's virtual slot 2 (vtable +0x08) with no arguments. Returns that
// call's result.
static int __fastcall CopyRulesBlockAndApply(int* self, void* /*edx*/, const void* sourceBlock) {
    // The original loads the vtable slot before the copy; the copy starts at
    // +0x18 and never touches the vtable pointer, but keep the same order.
    auto applyFn = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[2]);

    std::memcpy(reinterpret_cast<uint8_t*>(self) + 0x18, sourceBlock, 0x94);

    return applyFn(self, nullptr);
}

HOOK(0x0055A9F0, CopyRulesBlockAndApply, "auto");
