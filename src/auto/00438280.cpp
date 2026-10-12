#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00438280
// If a sub-object is stored at +0x4, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (0 when no sub-object).
static int __fastcall Hud_DestroyWidgetController(void* self, void* /*edx*/)
{
    int result = 0;
    uint32_t subRaw = CSF_AT(uint32_t, self, 0x4);
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(subRaw));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, sub, 0)[0])(sub, nullptr, 1);
        CSF_AT(uint32_t, self, 0x4) = 0;
    }
    return result;
}

HOOK(0x438280, Hud_DestroyWidgetController, "auto");
