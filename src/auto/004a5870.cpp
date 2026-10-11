#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees of the original (bound by address, signatures as given in the packet).
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindKeyIndexInTable, 0x00771B80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FallbackResolveTarget, 0x004A35D0, (void* self, void* edx, void* a2));

// Referenced vtable of S_00771b80 (the original passes &this+0x194 to 0x771b80 as its object).
[[maybe_unused]] static const void* const s_S00771b80Vtbl = CSF_VTBL(0x00771B80);

// Looks up a key in the object's table (ValuePointer + 0x194 .. 0x198 .. 0x198 + 12*n);
// on a hit with a valid value returns it, otherwise falls back to the secondary resolver
// using a2 (offset +88 when the owner reports state 1).
static int __fastcall C_00834598_GetLookupResultOrFallback(C_00834598* self, void* /*edx*/, S_00771b80* a2)
{
    // Owner object lives at self->field_0008; its +0x44 holds the handle object.
    C_008344f0* owner = reinterpret_cast<C_008344f0*>(self->field_0008);
    uint32_t handleObjectAddr = owner->field_0044;
    void* handleObject = reinterpret_cast<void*>(static_cast<uintptr_t>(handleObjectAddr));

    // vtbl slot 0xD8 / 4 = 54: returns the active C_00862490 entry (or null).
    auto getActiveEntry = reinterpret_cast<C_00862490* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, handleObject, 0)[54]);
    C_00862490* activeEntry = getActiveEntry(handleObject, nullptr);

    if (activeEntry != nullptr)
    {
        int valuePointer = Handle_GetValuePointer(activeEntry, nullptr);
        int keyIndex = FindKeyIndexInTable(reinterpret_cast<void*>(static_cast<uintptr_t>(valuePointer + 0x194)),
                                           nullptr, reinterpret_cast<int>(a2));
        if (keyIndex != -1)
        {
            int tableBase = CSF_AT(int, Handle_GetValuePointer(activeEntry, nullptr), 0x198);
            int result = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(tableBase)),
                                12 * keyIndex + 8);
            if (result != -1)
                return result;
        }
        // (falls through to the fallback with the original self)
    }

    // vtbl slot 0xFC / 4 = 63: returns an int state; 1 selects the +88 variant.
    auto getState = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, handleObject, 0)[63]);
    int stateValue = getState(handleObject, nullptr);

    int argOffset = (stateValue == 1) ? 88 : 0;
    void* adjustedArg = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(a2) + argOffset);
    return FallbackResolveTarget(self, nullptr, adjustedArg);
}

HOOK(0x4A5870, C_00834598_GetLookupResultOrFallback, "auto");
