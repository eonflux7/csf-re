#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObject_ProcessDeepestLeaf, 0x006B2AF0, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));

CSF_GLOBAL(float, g_fInv255, 0x00830A00);

// Fills a 4-float light color/intensity block. Defaults to 1.0 everywhere; when a
// scene object is given, the channels are scaled by its scene entry count (or,
// when that is -1, by the deepest-leaf value of its distance override owner) / 255.
static void __cdecl GfxLights_FillIntensityFromSceneEntry(int objectIndex, float* outColor)
{
    outColor[0] = 1.0f;
    outColor[1] = 1.0f;
    outColor[2] = 1.0f;
    outColor[3] = 1.0f;

    if (objectIndex == 0)
        return;

    int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
    int sceneEntryCount = AiCore_GetSceneEntryCount(
        reinterpret_cast<void*>(static_cast<intptr_t>(attachment)), nullptr);

    float intensity;
    if (sceneEntryCount == -1)
    {
        int distanceOwner = GfxSkin_GetDistanceOverrideOwner(objectIndex);
        if (distanceOwner == 0)
            return;

        uint32_t leafPointer = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<intptr_t>(distanceOwner)), 0x10);
        uint8_t leafValue = static_cast<uint8_t>(
            GfxObject_ProcessDeepestLeaf(reinterpret_cast<void*>(static_cast<intptr_t>(leafPointer)), nullptr));
        intensity = static_cast<float>(leafValue) * g_fInv255;
    }
    else
    {
        intensity = static_cast<float>(sceneEntryCount) * g_fInv255;
    }

    outColor[3] = 1.0f;
    outColor[1] = intensity;
    outColor[0] = intensity;
    outColor[2] = intensity;
}

HOOK(0x006D1730, GfxLights_FillIntensityFromSceneEntry, "auto");
