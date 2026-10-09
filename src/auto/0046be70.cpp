#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bound by address: 0x7697D0, one stack argument, ecx = object.
CSF_FUNC(int, __fastcall, MissionScript_LookupMaskedEntry, 0x007697D0, (void* self, void* edx, int a2));

// 0x46BE70: __stdcall(unsigned a1) with ecx = parser/owner object.
// When a1 has bit 0x800000 set, the object comes from this+0x48 and that bit is
// cleared from the value passed on. Otherwise the object is this->field_0044->field_0028.
static int __fastcall MissionScript_DispatchMaskedId(void* self, void* /*edx*/, unsigned int a1)
{
    // The original loads [this+0x44]->+0x28 before the test, so that read happens on every path.
    uint32_t defaultOwnerHolder = CSF_AT(uint32_t, self, 0x44);
    uint32_t defaultOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(defaultOwnerHolder)), 0x28);
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(defaultOwner));

    unsigned int v1 = a1;
    if ((a1 & 0x800000u) != 0)
    {
        v1 = a1 & 0xFF7FFFFFu;
        owner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x48)));
    }

    // Original: ecx = owner, edx = v1, push v1, call 0x7697D0.
    return MissionScript_LookupMaskedEntry(owner, reinterpret_cast<void*>(static_cast<uintptr_t>(v1)), static_cast<int>(v1));
}

HOOK(0x0046BE70, MissionScript_DispatchMaskedId, "auto");
