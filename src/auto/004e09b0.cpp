#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, GetScenePointPosition, 0x004DE860, (void* self, void* edx, int a2));

static uint32_t __fastcall ExpandScenePointXZBounds(
    uint32_t* self, void* /*edx*/,
    float* minX, float* minZ, float* maxX, float* maxZ)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x08);
    uint32_t index = 0;
    uint32_t recordOffset = 0;

    if (count != 0)
    {
        do
        {
            void* records = CSF_AT(void*, self, 0x0C);
            int pointId = CSF_AT(int, records, recordOffset);
            uint32_t sceneData = CGuiControl_GetField012C(
                static_cast<CGuiControl*>(g_pCurrentScene), nullptr);
            float* position = reinterpret_cast<float*>(GetScenePointPosition(
                reinterpret_cast<void*>(sceneData), nullptr, pointId));

            float x = CSF_AT(float, position, 0x00);
            float z = CSF_AT(float, position, 0x08);

            if (x > *maxX)
                *maxX = x;
            if (x < *minX)
                *minX = x;
            if (z > *maxZ)
                *maxZ = z;
            if (z < *minZ)
                *minZ = z;

            count = CSF_AT(uint32_t, self, 0x08);
            ++index;
            recordOffset += 16;
        } while (index < count);
    }

    return count;
}

HOOK(0x004E09B0, ExpandScenePointXZBounds, "auto");
