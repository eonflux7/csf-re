#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));

static void __fastcall C_0083f1d8_AccumulateNearbyEventStrength(
    C_0083f1d8* self, void* /*edx*/, void* event)
{
    void* context = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!context)
        return;

    const int eventType = CSF_AT(int, event, 0x04);
    if (eventType == 14 || eventType == 15)
    {
        if (CSF_AT(uint32_t, event, 0x44))
            return;
    }
    else
    {
        if (eventType != 4)
            return;
        if (CSF_AT(int, event, 0x10) < 0)
            return;

        const int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(event) + 0x08));
        if (!resolved)
            return;
        if (CSF_AT(int, reinterpret_cast<void*>(resolved), 0x50) != 4)
            return;
        if (CSF_AT(int, event, 0x60) != 12)
            return;
    }

    const float eventX = CSF_AT(float, event, 0x34);
    const float eventY = CSF_AT(float, event, 0x38);
    const float eventZ = CSF_AT(float, event, 0x3C);

    if (!(self->field_000c < self->field_0014))
        return;

    float* position = GetObjectPositionVector(context, nullptr);
    const float deltaZ = position[2] - eventZ;
    const float deltaY = position[1] - eventY;
    const float deltaX = position[0] - eventX;

    alignas(8) uint8_t displacementBytes[0x18];
    float* displacement = reinterpret_cast<float*>(displacementBytes);
    displacement[0] = deltaX;
    displacement[1] = deltaY;
    displacement[2] = deltaZ;

    const double distance = RwV3dLength(displacement);
    if (distance < self->field_0010)
    {
        const double strength =
            (self->field_0010 - distance) / self->field_0010 *
            self->field_0014;

        // The original also selects the new value on an unordered comparison.
        self->field_000c = strength < self->field_000c
            ? self->field_000c
            : static_cast<float>(strength);
    }
}

HOOK(0x004F9520, C_0083f1d8_AccumulateNearbyEventStrength, "auto");
