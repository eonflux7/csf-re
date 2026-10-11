#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4BE750, 0x004BE750, (void* self, void* edx, int a2));

// Returns the linked object's base (linked value - 4), or 0 when no object is linked.
// Each call re-invokes GetLinkedObjectValue, matching the original's repeated calls.
static uintptr_t LinkedObjectBaseOrZero(int a1) {
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (linkedValue) {
        return static_cast<uintptr_t>(linkedValue) - 4;
    }
    return 0;
}

static int __stdcall AiCore_FinDebugStep(int a1) {
    // Set bit 0 of the flags dword at +0x680 on the linked object.
    uintptr_t base = LinkedObjectBaseOrZero(a1);
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(base), 0x680) |= 1u;

    // Apply on the sub-object embedded at +0x440.
    base = LinkedObjectBaseOrZero(a1);
    int subObjectAddress = static_cast<int>(base + 0x440);
    UnnamedApply_4DC180(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr, subObjectAddress);

    // Address of the same sub-object, from a fresh lookup.
    base = LinkedObjectBaseOrZero(a1);
    int subObjectArg = static_cast<int>(base + 0x440);

    // Final dispatch: this = linked object base (or null), argument = sub-object address.
    base = LinkedObjectBaseOrZero(a1);
    return sub_4BE750(reinterpret_cast<void*>(base), nullptr, subObjectArg);
}

HOOK(0x004BF3D0, AiCore_FinDebugStep, "auto");
