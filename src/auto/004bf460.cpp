#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, sub_4BE750, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Returns the sub-object pointer at linked value - 4 (0 when the link is null), plus 0x440.
// Note: the original calls GetLinkedObjectValue on each use, so every step re-reads it.

static int __stdcall AiCore_ApplyLinkedSubobjectAndDispatch(void* a1, int /*a2*/, int /*a3*/)
{
    // Phase 1: apply to the linked sub-object at +0x440.
    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int linkedBase = linkedValue ? linkedValue - 4 : 0;
    UnnamedApply_4DC180(a1, nullptr, linkedBase + 0x440);

    // Phase 2: compute the +0x440 offset again for the dispatch argument.
    int linkedValueAgain = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int dispatchBase = linkedValueAgain ? linkedValueAgain - 4 : 0;
    int subobjectOffset = dispatchBase + 0x440;

    // Phase 3: dispatch on the linked object (this = linked - 4, or null if no link).
    int linkedValueFinal = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    if (linkedValueFinal)
    {
        return sub_4BE750(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValueFinal - 4)),
                          nullptr, subobjectOffset);
    }
    return sub_4BE750(nullptr, nullptr, subobjectOffset);
}

HOOK(0x004BF460, AiCore_ApplyLinkedSubobjectAndDispatch, "auto");
