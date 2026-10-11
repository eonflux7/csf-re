#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee (ret 4): the caller passes the field at +0x18 as `this` and a2 as its one stack argument.
CSF_FUNC(uint8_t*, __fastcall, BddLoader_ApplyScratchStateHelper, 0x00536A80, (void* self, void* edx, void* a2));

static int __fastcall BddLoader_ApplyScratchToGlobalState(void* self, void* /*edx*/, int a2)
{
    // ecx = [this+0x14] (a pointer to a sub-object whose vtable is queried)
    void* subObject = CSF_AT(void*, self, 0x14);
    void* scratchTarget = CSF_AT(void*, self, 0x18);

    // Virtual call at vtable slot 0x74 / 4 = 29: (subObject, scratchTarget, 0)
    void** vtable = CSF_AT(void**, subObject, 0);
    int result = ((int(__fastcall*)(void*, void*, void*, int))vtable[29])(
        subObject, nullptr, scratchTarget, 0);

    if (static_cast<uint8_t>(result) != 0) {
        // The original reloads [this+0x18] after the virtual call.
        void* reloadedTarget = CSF_AT(void*, self, 0x18);
        void* a2Arg = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            BddLoader_ApplyScratchStateHelper(reloadedTarget, nullptr, a2Arg)));
    }
    return result;
}

HOOK(0x0050E290, BddLoader_ApplyScratchToGlobalState, "auto");
