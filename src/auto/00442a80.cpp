#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveSkinObjectFromArg, 0x00681FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

static void* DynamicObj_AsObjectPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// Resolves the lookup value for a dynamic object slot.
// arg 0: resolve the skin object from the global value.
// arg 3..6: take the first element of the global value's container.
// Any other nonzero arg yields 0.
static int __cdecl DynamicObj_ResolveEntryLookupForArg(int a1)
{
    int element = 0;

    if (a1 != 0)
    {
        if (a1 <= 2 || a1 > 6)
            return 0;

        int globalValue = GetGlobalValue_4FBDE0();
        element = GetFirstElement(DynamicObj_AsObjectPointer(globalValue), nullptr);
    }
    else
    {
        int globalValue = GetGlobalValue_4FBDE0();
        element = ResolveSkinObjectFromArg(DynamicObj_AsObjectPointer(globalValue), nullptr);
    }

    if (element)
        return ResolveEntryLookupValue(DynamicObj_AsObjectPointer(element), nullptr);

    return 0;
}

HOOK(0x00442A80, DynamicObj_ResolveEntryLookupForArg, "auto");
