#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
static CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static CSF_GLOBAL(float, g_CloseTargetDistanceSquared, 0x00831148);
static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall S_004480f0_UpdateStationaryTime(
    S_004480f0* self, void* /*edx*/, float deltaTime)
{
    auto context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!context)
        return;

    float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(context), nullptr);

    const float dx = position[0] - self->field_004c;
    const float dy = position[1] - self->field_0050;
    const float dz = position[2] - self->field_0054;

    if (dz * dz + dx * dx + dy * dy > g_CloseTargetDistanceSquared)
    {
        self->field_004c = position[0];
        self->field_0050 = position[1];
        self->field_0054 = position[2];
        self->field_0058 = 0;
    }
    else
    {
        float elapsed;
        std::memcpy(&elapsed, &self->field_0058, sizeof(elapsed));
        elapsed = deltaTime + elapsed;
        std::memcpy(&self->field_0058, &elapsed, sizeof(elapsed));
    }
}

HOOK(0x004480F0, S_004480f0_UpdateStationaryTime, "auto");
