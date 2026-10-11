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

CSF_FUNC(void*, __fastcall, VehicleWheel_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, VehicleWheel_InitRatioVec4, 0x0040A5E0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, VehicleWheel_ApplyVec4, 0x0040B570, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __fastcall, VehicleWheel_BaseUpdate, 0x00419650, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(double, g_dWheelAtanY, 0x0082E278);

static void __fastcall C_0082e2e8_UpdateWheelSteering(C_0082e2e8* self, void* /*edx*/)
{
    VehicleWheel_BaseUpdate(self, nullptr);

    float steer = *reinterpret_cast<float*>(&self->field_0028);
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x30));
    if (CSF_AT(uint8_t, owner, 0x74))
        steer = -steer;

    // Original stack block: 4 floats of vector followed by the target angle at +0x10 (0x14 bytes total).
    float block[5];
    block[4] = 0.0f;
    if (steer != g_fZero) {
        block[4] = (float)atan2(g_dWheelAtanY, (double)*reinterpret_cast<float*>(&self->field_002c));
        if (steer <= g_fZero)
            block[4] = -block[4];
    }

    if (self->field_008c != block[4]) {
        float angleDelta = block[4] - self->field_008c;
        block[0] = 0.0f;
        block[1] = 0.0f;
        block[2] = 0.0f;
        block[3] = -1.0f;
        VehicleWheel_InitRatioVec4(block, nullptr, angleDelta, 0.30000001f);

        void* firstWheel = VehicleWheel_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 0);
        VehicleWheel_ApplyVec4(firstWheel, nullptr, 1, block);
        void* secondWheel = VehicleWheel_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 2);
        VehicleWheel_ApplyVec4(secondWheel, nullptr, 1, block);

        self->field_008c = block[4];
    }
}

HOOK(0x0041BB00, C_0082e2e8_UpdateWheelSteering, "auto");
