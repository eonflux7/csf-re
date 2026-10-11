#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxMatrix_RotateZAxisRadians, 0x006ADBE0, (int* self, void* , int a2, float a3));

CSF_GLOBAL(float, g_RadToDegFactor, 0x0082D7C4);

using VirtualRotateNotifyFn = int(__fastcall*)(void* self, void* edx, int* rotatedVec, float degrees, void* object);

static int __fastcall RotateTransformEntryZ(uint32_t* self, void* /*edx*/, int entryIndex, float angleRadians)
{
    uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    int** matrixSlot = reinterpret_cast<int**>(table + 28 * entryIndex + 8);
    int* sourceVector = *matrixSlot;

    int rotated[3];
    rotated[0] = sourceVector[0];
    rotated[1] = sourceVector[1];
    rotated[2] = sourceVector[2];

    int result = GfxMatrix_RotateZAxisRadians(
        reinterpret_cast<int*>(matrixSlot),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(rotated)),
        angleRadians);

    uint8_t* tableAfter = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
    void* object = *reinterpret_cast<void**>(tableAfter + 28 * entryIndex);
    if (object != nullptr) {
        float degrees = angleRadians * g_RadToDegFactor;
        VirtualRotateNotifyFn notify = reinterpret_cast<VirtualRotateNotifyFn>(CSF_AT(void**, object, 0)[72 / 4]);
        return notify(object, nullptr, rotated, degrees, object);
    }
    return result;
}

HOOK(0x004081A0, RotateTransformEntryZ, "auto");
