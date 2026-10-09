#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_497CD0, 0x00497CD0, (void* self, void* edx, int a2, int* a3, float* a4, void* a5, int a6, int a7, int a8));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, EulerAnglesToDirection, 0x006AE640, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Lazily initialises the cached target/aim data: resets the target slots, asks the
// search routine for a direction, and falls back to the object's own facing direction
// when no direction was produced.
static void __fastcall CPlayerCharacter_InitTargetDirection(CPlayerCharacter* self, void* /*edx*/)
{
    if (self->field_0268 != 0)
        return;

    self->field_0268 = 0;
    self->field_026c = 0;
    self->field_0274 = 0xFFFFFFFF;
    self->field_0278 = 0xFFFFFFFF;
    self->field_027c = 0xFFFFFFFF;

    float* targetPos = reinterpret_cast<float*>(&self->field_0280);
    targetPos[0] = g_ZeroVectorX;
    targetPos[1] = g_ZeroVectorY;
    targetPos[2] = g_ZeroVectorZ;

    self->field_0268 = 1;

    float* direction = &CSF_AT(float, self, 0x1C8);
    direction[0] = g_ZeroVectorX;
    direction[1] = g_ZeroVectorY;
    direction[2] = g_ZeroVectorZ;

    sub_497CD0(self, nullptr,
               reinterpret_cast<int>(&self->field_0270),
               reinterpret_cast<int*>(&self->field_027c),
               targetPos,
               direction,
               0, -1, 0);

    if (direction[0] == g_ZeroVectorX &&
        direction[1] == g_ZeroVectorY &&
        direction[2] == g_ZeroVectorZ)
    {
        float facingDir[3] = { 0.0f, 0.0f, 0.0f };
        float angles[2] = { 0.0f, 0.0f };

        void* anglesObj = ((void*(__fastcall*)(void*, void*, float*))CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr, angles);
        float* result = EulerAnglesToDirection(anglesObj, nullptr, facingDir);

        direction[0] = result[0];
        direction[1] = result[1];
        direction[2] = result[2];
        NormalizeVector3(direction, nullptr);
    }
}

HOOK(0x00497FB0, CPlayerCharacter_InitTargetDirection, "auto");
