#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __fastcall, CPlayerCharacter_ApplyTransitionValue, 0x00492500,
         (void* self, void* edx, float a2));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (void* self, void* edx));

static char __fastcall CPlayerCharacter_UpdateTransition(
    CPlayerCharacter* self, void* /*edx*/, float progress)
{
    char result = CSF_AT(char, self, 0x184);
    if (!result)
        return result;

    CSF_AT(float, self, 0x188) = progress;
    if (progress >= g_TransitionOne)
    {
        const uint32_t finalState = self->field_03ec;
        CSF_AT(uint32_t, self, 0x1C4) = finalState;

        uint32_t flags = self->field_02e0;
        CSF_AT(char, self, 0x184) = 0;
        CSF_AT(float, self, 0x188) = 1.0f;

        if (finalState == 1)
            flags |= 4;
        else
            flags &= 0xFFFFFFFB;

        void* controller = reinterpret_cast<void*>(self->field_0030);
        self->field_02e0 = flags;
        CSF_AT(uint32_t, controller, 0x20) = 0;
    }

    const float value =
        (CSF_AT(float, self, 0x1A8) - CSF_AT(float, self, 0x1A4)) *
            CSF_AT(float, self, 0x188) +
        CSF_AT(float, self, 0x1A4);

    result = static_cast<char>(
        CPlayerCharacter_ApplyTransitionValue(self, nullptr, value));

    void* attachedObject = reinterpret_cast<void*>(self->field_004c);
    if (attachedObject)
    {
        alignas(8) uint8_t positionBytes[0xC];
        float* position = reinterpret_cast<float*>(positionBytes);

        const float deltaX =
            CSF_AT(float, self, 0x198) - CSF_AT(float, self, 0x18C);
        const float blend = CSF_AT(float, self, 0x188);
        const float deltaY =
            CSF_AT(float, self, 0x19C) - CSF_AT(float, self, 0x190);
        const float deltaZ =
            CSF_AT(float, self, 0x1A0) - CSF_AT(float, self, 0x194);

        position[0] = deltaX * blend + CSF_AT(float, self, 0x18C);
        position[1] = deltaY * blend + CSF_AT(float, self, 0x190);
        position[2] = deltaZ * blend + CSF_AT(float, self, 0x194);

        void* target =
            C_0082dd78_DispatchReturnedObject(attachedObject, nullptr);
        return ((char(__fastcall*)(void*, void*, float*))
                    CSF_AT(void**, target, 0)[0x2C / 4])(
            target, nullptr, position);
    }

    return result;
}

HOOK(0x00493D50, CPlayerCharacter_UpdateTransition, "auto");
