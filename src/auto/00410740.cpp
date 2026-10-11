#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* minimum, float* maximum));
CSF_FUNC(float*, __cdecl, SelectMovementRateBound, 0x0040A640, (float* a1, float* a2));
CSF_FUNC(char, __fastcall, C_0082dd78_UpdateState, 0x00410290, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MovementRateBound, 0x0082DEA8);
CSF_GLOBAL(float, g_HeightSnapDistance, 0x0082DEAC);
CSF_GLOBAL(float, g_HeightTolerance, 0x0082DEB0);
CSF_GLOBAL(float, g_DownwardMovementScale, 0x0082DF20);

static void __fastcall C_0082deb8_UpdateVerticalPosition(
    C_0082deb8* self, void* /*edx*/, float elapsedTime)
{
    C_0082dd78_UpdateState(self, nullptr);

    if (self->field_0034 == 0xbf800000)
        return;

    float* targetHeight = reinterpret_cast<float*>(&self->field_0034);
    void* sourcePosition = reinterpret_cast<void*>(
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));

    struct Position
    {
        int x;
        float y;
        float z;
    };

    Position position;
    position.x = CSF_AT(int, sourcePosition, 0x0);
    position.y = CSF_AT(float, sourcePosition, 0x4);
    const float difference = *targetHeight - position.y;
    position.z = CSF_AT(float, sourcePosition, 0x8);

    float movement[2] = {std::fabs(difference), difference};
    if (!(movement[0] > g_HeightTolerance))
        return;

    void* object = reinterpret_cast<void*>(self->field_0004);
    void* controller =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
    void* state =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x68 / 4])(controller, nullptr);
    const char snapRequested =
        reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x50 / 4])(state, nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0) ||
        self->field_003c ||
        snapRequested ||
        !(movement[0] > g_HeightSnapDistance))
    {
        position.y = *targetHeight;
        self->field_003c = 0;
    }
    else
    {
        object = reinterpret_cast<void*>(self->field_0004);
        void* movementController = CSF_AT(void*, object, 0x48);
        movement[0] = static_cast<float>(
            reinterpret_cast<double (__fastcall*)(void*, void*)>(
                CSF_AT(void**, movementController, 0)[0x48 / 4])(
                    movementController, nullptr));

        float* rate = SelectMovementRateBound(movement, &g_MovementRateBound);
        float step = *rate * elapsedTime;
        if (!(movement[1] > g_fZero))
            step *= g_DownwardMovementScale;

        elapsedTime = step + position.y;
        ClampFloatToBounds(&elapsedTime, &position.y, targetHeight);
        position.y = elapsedTime;
    }

    SceneLoader_ConfigureFocusEntry(
        reinterpret_cast<void*>(self->field_0004), nullptr, &position.x);
}

HOOK(0x00410740, C_0082deb8_UpdateVerticalPosition, "auto");
