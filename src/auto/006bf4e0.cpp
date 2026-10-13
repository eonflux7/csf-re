#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00860700_DispatchDeltaFromAnchor(C_00860700* self, void* /*edx*/, float* a2)
{
    float* anchor = reinterpret_cast<float*>(static_cast<uintptr_t>(self->vGetField000cPlus64()));

    float delta[3];
    delta[0] = a2[0] - anchor[0];
    delta[1] = a2[1] - anchor[1];
    delta[2] = a2[2] - anchor[2];

    // Slot 0x58 returns a full eax (int), so call it through the raw vtable and keep all 32 bits.
    auto slot58 = reinterpret_cast<int(__fastcall*)(void*, void*, float*)>(CSF_AT(void**, self, 0)[0x58 / 4]);
    return slot58(self, nullptr, delta);
}

HOOK(0x6BF4E0, C_00860700_DispatchDeltaFromAnchor, "auto");
