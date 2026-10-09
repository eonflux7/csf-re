#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Lookup routine, __thiscall (self in ecx), two stack args, ret 8.
CSF_FUNC(uint32_t*, __fastcall, sub_782330, 0x00782330, (void* self, void* edx, int a2, uint32_t* a3));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Looks up a float vector by id, and if its y component is nonzero, sets the
// dirty flag at +0xA8 of the target and writes three values scaled by the
// float at +0x10 of this->field_1fb8 into +0xAC..+0xB4.
static void __fastcall MissionSetup_WriteScaledVectorFromLookup(S_004e9090* self, void* /*edx*/, int lookupId, int targetAddr)
{
    float* lookupVec = reinterpret_cast<float*>(sub_782330(reinterpret_cast<void*>(self->field_1fb8), nullptr, lookupId, nullptr));
    if (lookupVec == nullptr) {
        return;
    }
    if (lookupVec[1] == g_fZero) {
        return;
    }

    float scale = CSF_AT(float, reinterpret_cast<void*>(self->field_1fb8), 0x10);
    auto* target = reinterpret_cast<uint8_t*>(targetAddr);

    CSF_AT(uint8_t, target, 0xA8) = 1;
    CSF_AT(float, target, 0xAC) = scale * lookupVec[1];
    CSF_AT(float, target, 0xB0) = scale * lookupVec[2];
    CSF_AT(float, target, 0xB4) = scale * lookupVec[3];
}

HOOK(0x004E9090, MissionSetup_WriteScaledVectorFromLookup, "auto");
