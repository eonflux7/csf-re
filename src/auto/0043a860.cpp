#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Fades the alpha of a minimap element according to its vertical distance from the scene position.
static void __stdcall Minimap_FadeByHeightDistance(void* a1, int* a2)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    float* position = GetObjectPositionVector(reinterpret_cast<void*>(sceneMenuContext), nullptr);

    float posY = position[1];
    float distance = fabsf(CSF_AT(float, a1, 4) - posY);

    float nearLimit = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x1834);
    float range = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x1838);
    float minAlpha = CSF_AT(float, GetGlobalValue_8BC1E0(), 0x183C);

    if (distance > nearLimit) {
        float alpha;
        if (range + nearLimit < distance) {
            alpha = (float)((double)minAlpha * (double)g_ColorChannelScale);
        } else {
            double scale = (double)g_ColorChannelScale;
            alpha = (float)(scale - (scale - (double)minAlpha * scale) * (double)((distance - nearLimit) / range));
        }

        void** vtbl = *reinterpret_cast<void***>(a2);
        float* current = ((float*(__fastcall*)(void*, void*))vtbl[0x6C / 4])(a2, nullptr);

        float color[4];
        color[0] = current[0];
        color[1] = current[1];
        color[2] = current[2];
        color[3] = current[3];
        color[3] = alpha;

        void** vtbl2 = *reinterpret_cast<void***>(a2);
        ((void(__fastcall*)(void*, void*, float*))vtbl2[0x68 / 4])(a2, nullptr, color);
    }
}

HOOK(0x0043A860, Minimap_FadeByHeightDistance, "auto");
