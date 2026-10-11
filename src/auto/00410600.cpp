#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_UpdateFocus, 0x00410290, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static char __fastcall C_0082deb8_SetFocusPosition(
    C_0082deb8* self, void* /*edx*/, const float* position)
{
    float focusPosition[3];
    std::memcpy(focusPosition, position, sizeof(focusPosition));
    self->field_0038 = CSF_AT(uint32_t, position, 0x4);

    const float directionX = focusPosition[0] - self->field_0018;
    const float directionY = focusPosition[1] - self->field_001c;
    const float directionZ = focusPosition[2] - self->field_0020;

    std::memcpy(&self->field_0024, &directionX, sizeof(directionX));
    std::memcpy(&self->field_0028, &directionY, sizeof(directionY));
    CSF_AT(float, self, 0x2C) = directionZ;

    if (FloatFromBits(self->field_0024) != g_ZeroVectorX ||
        FloatFromBits(self->field_0028) != g_ZeroVectorY ||
        CSF_AT(float, self, 0x2C) != g_ZeroVectorZ)
    {
        NormalizeVector3(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);
    }

    void* object = reinterpret_cast<void*>(self->field_0004);
    void* focusEntry =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);

    if (CSF_AT(uint32_t, focusEntry, 0x78) == 0xBF800000)
    {
        return static_cast<char>(SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr,
            reinterpret_cast<int*>(focusPosition)));
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (!CSF_AT(uint8_t, globalState, 0xD0))
    {
        void* objectPosition = reinterpret_cast<void*>(GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr));
        std::memcpy(&focusPosition[1],
                    reinterpret_cast<uint8_t*>(objectPosition) + 0x4,
                    sizeof(float));
    }

    SceneLoader_ConfigureFocusEntry(
        reinterpret_cast<void*>(self->field_0004), nullptr,
        reinterpret_cast<int*>(focusPosition));
    return C_0082dd78_UpdateFocus(self, nullptr);
}

HOOK(0x00410600, C_0082deb8_SetFocusPosition, "auto");
