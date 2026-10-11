#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x4E5A30: bool __fastcall (self, edx, float* a3, uint32_t* a4), pops 8.
CSF_FUNC(bool, __fastcall, sub_4E5A30, 0x004E5A30, (void* self, void* edx, float* a3, uint32_t* a4));

// Builds a 6-dword block: the object's first three dwords (vtable pointer, field_0004,
// field_0008) followed by the three dwords at a2. Passes that block to 0x4E5A30 with a
// null fourth argument. The original copies the raw dwords, so the bits are copied as uint32_t.
static bool __fastcall C_0083e9d0_TestPairWithSelfVector(C_0083e9d0* self, void* /*edx*/, float* a2, float* a3)
{
    uint32_t words[6];
    words[0] = CSF_AT(uint32_t, self, 0x0);
    words[1] = self->field_0004;
    words[2] = self->field_0008;
    words[3] = CSF_AT(uint32_t, a2, 0x0);
    words[4] = CSF_AT(uint32_t, a2, 0x4);
    words[5] = CSF_AT(uint32_t, a2, 0x8);

    float* block = reinterpret_cast<float*>(words);
    (void)a3;
    return sub_4E5A30(self, nullptr, block, nullptr);
}

HOOK(0x004E5C30, C_0083e9d0_TestPairWithSelfVector, "auto");
