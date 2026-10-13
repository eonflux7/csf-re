#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"

// Wrapper for the virtual call at vtable slot 0x50 (index 20) on the object
// stored at self+4. The 16-bit argument is sign-extended to int before the call.
static int __fastcall ForwardShortToInnerVirtualSlot20(void* self, void* /*edx*/, int16_t a2)
{
    void* innerObject = CSF_AT(void*, self, 0x4);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, innerObject, 0)[0x50 / 4])(
        innerObject, nullptr, static_cast<int>(a2));
}

HOOK(0x006CA560, ForwardShortToInnerVirtualSlot20, "auto");
