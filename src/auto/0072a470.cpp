#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Forwards to the virtual method at vtable byte offset +0x60 (slot 24) of the
// passed control object and returns its result; returns 0 for a null pointer.
static int __stdcall CGuiPanel_InvokeVirtualSlot60(int a1)
{
    if (a1 == 0)
        return 0;

    void** vtbl = CSF_AT(void**, a1, 0);
    return ((int(__fastcall*)(void*, void*))vtbl[0x60 / 4])(reinterpret_cast<void*>(a1), nullptr);
}

HOOK(0x0072A470, CGuiPanel_InvokeVirtualSlot60, "auto");
