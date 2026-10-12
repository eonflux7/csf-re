#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TraceGroundWithState, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, C_008342e8_OnSceneObjectUpdated, 0x004959A0, (C_008342e8* self, void* , void* updatedObject));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_MovementScale, 0x0082DD6C);
CSF_GLOBAL(float, g_SpeedConversion, 0x0082DD70);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float& MovementFloat(uint32_t& field)
{
    return reinterpret_cast<float&>(field);
}

static uint32_t __fastcall C_0082dd78_UpdateGroundMovement(
    C_0082dd78* self, void* /*edx*/, float deltaTime)
{
    alignas(8) uint8_t traceBytes[0x8] = {};
    alignas(8) uint8_t focusBytes[0x5C] = {};
    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t displacementBytes[0x10];

    float* trace = reinterpret_cast<float*>(traceBytes);
    float* position = reinterpret_cast<float*>(positionBytes);
    float* displacement = reinterpret_cast<float*>(displacementBytes);

    position[0] = std::sin(MovementFloat(self->field_0044))
                * MovementFloat(self->field_0050);
    position[2] = std::cos(MovementFloat(self->field_0044))
                * MovementFloat(self->field_0050);

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    MovementFloat(self->field_0064) =
        (position[0] - MovementFloat(self->field_0064))
        * CSF_AT(float, settings, 0x1D78)
        + MovementFloat(self->field_0064);

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    MovementFloat(self->field_0068) =
        -MovementFloat(self->field_0068) * CSF_AT(float, settings, 0x1D78)
        + MovementFloat(self->field_0068);

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    MovementFloat(self->field_006c) =
        (position[2] - MovementFloat(self->field_006c))
        * CSF_AT(float, settings, 0x1D78)
        + MovementFloat(self->field_006c);

    if (MovementFloat(self->field_0050) < g_fZero)
        MovementFloat(self->field_0054) = -MovementFloat(self->field_0054);

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    MovementFloat(self->field_0058) =
        (MovementFloat(self->field_0054) - MovementFloat(self->field_0058))
        * CSF_AT(float, settings, 0x1D7C)
        + MovementFloat(self->field_0058);

    displacement[3] =
        MovementFloat(self->field_0064) * MovementFloat(self->field_0064)
        + MovementFloat(self->field_0068) * MovementFloat(self->field_0068)
        + MovementFloat(self->field_006c) * MovementFloat(self->field_006c);

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const float speed = std::sqrt(displacement[3]);
    const float speedScale =
        CSF_AT(float, settings, 0x1D70) * g_SpeedConversion;

    self->field_0050 = 0;
    self->field_0054 = 0;
    CSF_AT(float, focusBytes, 0x24) = 1.0f;
    CSF_AT(uint8_t, focusBytes, 0x40) = 1;

    const float angleChange =
        speed / (speedScale * g_MovementScale)
        * MovementFloat(self->field_0058) * deltaTime;
    self->field_0048 = angleChange;
    MovementFloat(self->field_0044) =
        angleChange + MovementFloat(self->field_0044);

    position[0] = deltaTime * MovementFloat(self->field_0064);
    displacement[0] = position[0];
    position[1] = deltaTime * MovementFloat(self->field_0068);
    displacement[1] = position[1];
    position[2] = deltaTime * MovementFloat(self->field_006c);
    displacement[2] = position[2];

    if (displacement[2] * displacement[2]
        + displacement[0] * displacement[0]
        + displacement[1] * displacement[1] > g_fZero)
    {
        int attempts = 0;
        for (;;)
        {
            TraceGroundWithState(
                reinterpret_cast<void*>(self->field_0008), nullptr,
                displacement, trace,
                reinterpret_cast<int*>(&self->field_0070));

            CSF_AT(uint32_t, focusBytes, 0x04) = self->field_005c;
            SceneLoader_ConfigureFocusEntry(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                reinterpret_cast<int*>(focusBytes));

            void* returnedObject =
                C_0082dd78_DispatchReturnedObject(self, nullptr);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
                    returnedObject, nullptr);

            ++attempts;
            displacement[0] = CSF_AT(float, focusBytes, 0x0C);
            displacement[2] = CSF_AT(float, focusBytes, 0x14);
            displacement[1] = 0.0f;

            if (attempts > 4)
            {
                position[2] = CSF_AT(float, focusBytes, 0x08);
                position[1] = CSF_AT(float, focusBytes, 0x04)
                            + g_flt_0082DC84;
                position[0] = CSF_AT(float, focusBytes, 0x00);
                SceneLoader_ConfigureFocusEntry(
                    reinterpret_cast<void*>(self->field_0004), nullptr,
                    reinterpret_cast<int*>(positionBytes));

                returnedObject =
                    C_0082dd78_DispatchReturnedObject(self, nullptr);
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
                        returnedObject, nullptr);
                break;
            }

            if (!(displacement[2] * displacement[2]
                  + displacement[0] * displacement[0] > g_fZero))
                break;
        }
    }

    uint32_t count = CSF_AT(uint32_t, g_pCurrentScene, 0x68);
    uint8_t* entries = reinterpret_cast<uint8_t*>(g_pCurrentScene) + 0x60;
    uint32_t index = 0;
    if (count)
    {
        do
        {
            C_008342e8_OnSceneObjectUpdated(
                CSF_AT(C_008342e8**, entries, 0)[index], nullptr,
                reinterpret_cast<void*>(self->field_0004));
            count = CSF_AT(uint32_t, entries, 0x08);
            ++index;
        } while (index < count);
    }
    return count;
}

HOOK(0x0040E510, C_0082dd78_UpdateGroundMovement, "auto");
