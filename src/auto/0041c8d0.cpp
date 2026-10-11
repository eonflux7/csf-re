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

// Callees bound by address (signatures per packet).
CSF_FUNC(void*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, InitRatioVec4, 0x0040A5E0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, C_0082daf4_ApplyKeyVector, 0x0040B550, (void* self, void* edx, int a2, float* a3));

// Sets the two wheel-rotation keys (23 = -pi/2, 24 = +pi/2) on the vehicle's
// animation table, then clears the dirty byte of the wheel record at index a2.
// Returns 3 * a2 (the record stride index).
static int __fastcall C_0082e3b8_ConfigureWheelRotations(C_0082e3b8* self, void* /*edx*/, int a2)
{
    float rotationVec[4];
    rotationVec[0] = 0.0f;
    rotationVec[1] = 0.0f;
    rotationVec[2] = 0.0f;
    rotationVec[3] = -1.0f;

    InitRatioVec4(rotationVec, nullptr, -1.5707964f, 1.0f);
    void* keyFirst = C_0082daf4_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 23);
    C_0082daf4_ApplyKeyVector(keyFirst, nullptr, 1, rotationVec);

    InitRatioVec4(rotationVec, nullptr, 1.5707964f, 1.0f);
    void* keySecond = C_0082daf4_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 24);
    C_0082daf4_ApplyKeyVector(keySecond, nullptr, 1, rotationVec);

    uint8_t* wheelRecords = reinterpret_cast<uint8_t*>(self->field_0010);
    wheelRecords[24 * a2 + 12] = 0;
    return 3 * a2;
}

HOOK(0x0041C8D0, C_0082e3b8_ConfigureWheelRotations, "auto");
