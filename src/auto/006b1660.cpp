#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x006B1660
// If a sub-object is stored at +0x8C, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (0 when no sub-object).
static int __fastcall GlobalSlot_ApplyAndClear(void* self, void* /*edx*/)
{
    int result = 0;
    uint32_t subRaw = CSF_AT(uint32_t, self, 0x8C);
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(subRaw));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, sub, 0)[0])(sub, nullptr, 1);
        CSF_AT(uint32_t, self, 0x8C) = 0;
    }
    return result;
}

HOOK(0x6B1660, GlobalSlot_ApplyAndClear, "auto");
