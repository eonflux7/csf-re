#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, Task_GetTargetPosition, 0x004D1520, (void* self, void* edx));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(double, g_dThreeQuarterPi, 0x0082FAC0);
CSF_GLOBAL(float, g_fQuarterPi, 0x0082FAC8);

static int __fastcall TaskState_SelectTurnState(S_00421750* self, void* /*edx*/)
{
    int result = -1;
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);

    char* nameBuf = Task_GetTargetNameBuffer(reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x34)), nullptr);
    double angle = g_fZero;

    if (*nameBuf) {
        float* target = reinterpret_cast<float*>(Task_GetTargetPosition(nameBuf, nullptr));
        float vec[3];
        vec[2] = target[2] - self->field_0220;
        vec[1] = target[1] - self->field_021c;
        vec[0] = target[0] - self->field_0218;
        float rawAngle = (float)S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(vec), nullptr);
        angle = AngleDiffNormalized(rawAngle, self->field_023c);
    }

    double absAngle = fabs(angle);
    if (absAngle < (double)g_fQuarterPi) {
        owner = reinterpret_cast<uint8_t*>(self->field_0014);
        uint32_t state = CSF_AT(uint32_t, owner, 0x1E0);
        if (state == 0)
            return 1;
        if (state == 1)
            return 3;
        if (state == 2)
            return 23;
        return result;
    }

    if (absAngle >= g_dThreeQuarterPi) {
        owner = reinterpret_cast<uint8_t*>(self->field_0014);
        uint32_t state = CSF_AT(uint32_t, owner, 0x1E0);
        if (state == 0)
            return 2;
        if (state == 1 || state == 2)
            return 4;
        return result;
    }

    if (angle <= (double)g_fZero)
        return 6;
    return 5;
}

HOOK(0x00421750, TaskState_SelectTurnState, "auto");
