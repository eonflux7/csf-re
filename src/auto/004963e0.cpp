#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_4053A0, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(uint32_t*, __fastcall, sub_40E070, 0x0040E070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(char, __fastcall, sub_493D50, 0x00493D50, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fGroundProbeOffset, 0x00833E04); // 15.0f

// When the flag at +0x184 is set, probes the world below the target height;
// if nothing is hit (or the hit value is not negative) triggers the 1.0 action.
static void __fastcall C_00833ee8_CheckGroundProbe(C_00833ee8* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0x184))
        return;

    // Collision/trace result object (0x70 bytes of room on the original stack).
    float traceResult[28];
    sub_40E070(traceResult, nullptr);

    int traceFlags = 3;

    // The target height lives inside the trace object (offset 0x68) and is
    // re-read from there after the calls below.
    traceResult[26] = (CSF_AT(float, self, 0x19c) - CSF_AT(float, self, 0x190)) + CSF_AT(float, self, 0x190);

    void* owner = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));
    void* ownerData = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x30 / 4])(owner, nullptr);

    float probePos[3];
    probePos[0] = 0.0f;
    probePos[2] = 0.0f;
    probePos[1] = (traceResult[26] - CSF_AT(float, ownerData, 4)) + g_fGroundProbeOffset;

    void* world = CSF_AT(void*, reinterpret_cast<void*>(self->field_004c), 8);

    if (!sub_4053A0(world, nullptr, probePos, traceResult, &traceFlags) || !(traceResult[12] < g_fZero))
        sub_493D50(self, nullptr, 1.0f);
}

HOOK(0x004963E0, C_00833ee8_CheckGroundProbe, "auto");
