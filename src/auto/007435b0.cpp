// Commandos: Strike Force - net callbacks module.
// If the object stored at +0xD4 is non-null, the original tail-jumps to 0x4E1510
// with that object as `this`, forwarding its eax as the result. Otherwise it returns
// without calling anything.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x4E1510 : __thiscall, no stack args (ret 0). Body not yet reimplemented.
CSF_FUNC(int, __fastcall, C_0083e248_ApplyDefaultDescriptorAndGetState, 0x004E1510, (void* self, void* edx));

static int __fastcall NetCallbacks_ForwardApplyDefaultDescriptor(void* self, void* /*edx*/)
{
    // The original loads the pointer at +0xD4 into ecx and tail-jumps, so the callee
    // receives that object (not `self`) as `this`, and its eax is returned as-is.
    uint32_t pDescriptorObject = CSF_AT(uint32_t, self, 0xD4);
    if (pDescriptorObject != 0)
    {
        return C_0083e248_ApplyDefaultDescriptorAndGetState(reinterpret_cast<void*>(pDescriptorObject), nullptr);
    }

    // Original leaves eax untouched on this path; the caller-visible value is not
    // reproducible in C++, so 0 is returned.
    return 0;
}

HOOK(0x7435B0, NetCallbacks_ForwardApplyDefaultDescriptor, "auto");
