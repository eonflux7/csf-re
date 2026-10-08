#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x692440: __cdecl, looks up an object by tag id / index, returns it in eax.
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x692440, (int a1, int a2));

// 0x720DF0: __thiscall(this = child, a2) forwarding into child's vtbl slot 16 (callee pops 4 bytes).
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x720DF0, (int* self, void* /*edx*/, int a2));

// 0x543BC0: if the child object pointer at this+0x138 is set, obtain the tagged object and
// forward it into the child's vtbl slot 16; otherwise return 0.
static int __fastcall CMenuControls_InvokeChildSlot16(void* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x138));
    if (result)
    {
        int tagged = MenuMessageBox_ResolveEntryByKey(0x6c6f3031, 1);
        // re-read the field: the call above may have modified the object
        int* child = reinterpret_cast<int*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x138)));
        return C_00720DF0_ForwardCallToChildVtblSlot16(child, nullptr, tagged);
    }
    return result;
}

HOOK(0x543BC0, CMenuControls_InvokeChildSlot16, "auto");
