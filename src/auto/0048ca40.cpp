#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// g_HalfDurationScale @ 0x0082D444 (float), multiplied into the attack step limit.
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall AttackTasks_SetCoverAttackPosition(void* self, void* /*edx*/, const uint32_t* a2)
{
    // Clear bit 0 and set bit 1 of the flags byte at +0x39.
    CSF_AT(uint8_t, self, 0x39) = static_cast<uint8_t>((CSF_AT(uint8_t, self, 0x39) & 0xFE) | 0x02);

    // Copy the three dwords of the source vector into the block starting at +0x14.
    CSF_AT(uint32_t, self, 0x14) = a2[0];
    CSF_AT(uint32_t, self, 0x18) = a2[1];
    CSF_AT(uint32_t, self, 0x1C) = a2[2];

    void* target = CSF_AT(void*, self, 0x04);
    CSF_AT(float, self, 0x3C) = 2.0f;

    // Virtual call at vtable slot 0xA4 / 4 = 41; writes a vector into outVec (12 bytes).
    alignas(8) uint8_t outVec[12];
    using QueryFn = void* (__fastcall*)(void* self, void* edx, void* outBuf);
    QueryFn queryPosition = reinterpret_cast<QueryFn>(CSF_AT(void**, target, 0)[0xA4 / 4]);
    uint8_t* result = static_cast<uint8_t*>(queryPosition(target, nullptr, outVec));

    float positionDelta = CSF_AT(float, result, 0x04) - CSF_AT(float, self, 0x18);
    float maxStep = CSF_AT(float, target, 0x1D4) * g_HalfDurationScale;
    if (positionDelta > maxStep)
    {
        CSF_AT(float, self, 0x18) = CSF_AT(float, target, 0x1D4) + CSF_AT(float, self, 0x18);
    }
}

HOOK(0x0048CA40, AttackTasks_SetCoverAttackPosition, "auto");
