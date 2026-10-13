#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(uint32_t*, __fastcall, CHud_CopyBoundsFromSource, 0x0042E150, (CHud* self, void* , int* a2));
CSF_FUNC(int, __fastcall, CHud_AttachPanelsToTarget, 0x0042E1A0, (CHud* self, void* , int a2));
CSF_FUNC(float*, __fastcall, Minimap_ComputeScaledDirection, 0x0043A9C0, (C_00830e5c* self, void* , float* out, float* to, float* from, float refLength, uint32_t absolute));
CSF_FUNC(int, __fastcall, C_00830e5c_IsPointWithinRadiusOfMenuObject, 0x0043AAD0, (C_00830e5c* self, void* , float* a2));
CSF_FUNC(char, __fastcall, C_00830e5c_RenderRadarWidget, 0x0043AF30, (C_00830e5c* self, void* , void* item));
CSF_FUNC(double, __stdcall, Minimap_GetVectorAngleDegrees, 0x0043B380, (float* vec));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStatusIcons_TryLookupStatusValue, 0x004588D0, (S_00458840* self, void* , int a2, uint32_t* a3));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fInv255, 0x00830A00);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int* __fastcall C_00830e5c_DrawMarkerAt(
    C_00830e5c* self, void* /*edx*/, float* position,
    float heading, S_00458840* status, float opacity)
{
    void* context = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));

    uint32_t absolute = static_cast<uint32_t>(
        C_00830e5c_IsPointWithinRadiusOfMenuObject(self, nullptr, position));
    // The original negates only AL, leaving the upper bytes intact.
    absolute = (absolute & 0xFFFFFF00u) |
        static_cast<uint32_t>(static_cast<uint8_t>(absolute) == 0);

    float referenceLength = static_cast<float>(self->vGetValue());
    float* contextPosition = GetObjectPositionVector(context, nullptr);
    float direction[2];
    Minimap_ComputeScaledDirection(
        self, nullptr, direction, contextPosition, position,
        referenceLength, absolute);

    uint32_t bounds[4] = {0, 0, 0, 0};
    int statusId = reinterpret_cast<int>(status);
    int lookupId = statusId;
    if (statusId == 4 || statusId == 7)
    {
        if (static_cast<uint8_t>(
                C_00830e5c_IsPointWithinRadiusOfMenuObject(
                    self, nullptr, position)) == 0)
            lookupId = 11;
    }

    int* widget = reinterpret_cast<int*>(
        HudStatusIcons_TryLookupStatusValue(
            reinterpret_cast<S_00458840*>(self->field_0004),
            nullptr, lookupId, bounds));
    if (!widget)
        return widget;

    CHud_CopyBoundsFromSource(
        reinterpret_cast<CHud*>(GetGlobal8afc78()), nullptr, widget);

    uint32_t pair[2] = {bounds[0], bounds[1]};
    ((void (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, widget, 0)[0x28 / 4])(widget, nullptr, pair);

    pair[0] = bounds[2] - bounds[0];
    pair[1] = bounds[3] - bounds[1];
    ((void (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, widget, 0)[0x38 / 4])(widget, nullptr, pair);

    // The item consists of a widget, heading, direction, and two flags.
    alignas(8) uint8_t itemBytes[0x14];
    CSF_AT(int*, itemBytes, 0x00) = widget;
    CSF_AT(float, itemBytes, 0x04) = heading;
    CSF_AT(float, itemBytes, 0x08) = direction[0];
    CSF_AT(float, itemBytes, 0x0C) = direction[1];
    CSF_AT(uint8_t, itemBytes, 0x10) = 0;
    CSF_AT(uint8_t, itemBytes, 0x11) = 1;

    if ((statusId == 4 || statusId == 7) &&
        static_cast<uint8_t>(
            C_00830e5c_IsPointWithinRadiusOfMenuObject(
                self, nullptr, position)) == 0)
    {
        CSF_AT(uint8_t, itemBytes, 0x10) = 1;
        CSF_AT(float, itemBytes, 0x04) = static_cast<float>(
            -Minimap_GetVectorAngleDegrees(
                reinterpret_cast<float*>(itemBytes + 0x08)));
    }
    else
    {
        CSF_AT(uint8_t, itemBytes, 0x10) = 0;
    }

    float* sourceColor = ((float* (__fastcall*)(void*, void*))
        CSF_AT(void**, widget, 0)[0x6C / 4])(widget, nullptr);
    float opaqueColor[4];
    std::memcpy(opaqueColor, sourceColor, 3 * sizeof(float));
    opaqueColor[3] = 255.0f;
    ((void (__fastcall*)(void*, void*, float*))
        CSF_AT(void**, widget, 0)[0x68 / 4])(widget, nullptr, opaqueColor);

    if (statusId == 4 || statusId == 7)
    {
        ((void (__fastcall*)(C_00830e5c*, void*, float*, int*))
            self->vtbl[0x0C / 4])(self, nullptr, position, widget);
    }

    sourceColor = ((float* (__fastcall*)(void*, void*))
        CSF_AT(void**, widget, 0)[0x6C / 4])(widget, nullptr);
    float fadedColor[4];
    std::memcpy(fadedColor, sourceColor, sizeof(fadedColor));

    sourceColor = ((float* (__fastcall*)(void*, void*))
        CSF_AT(void**, widget, 0)[0x6C / 4])(widget, nullptr);
    std::memcpy(fadedColor, sourceColor, sizeof(fadedColor));
    fadedColor[3] = opacity * g_fInv255 * fadedColor[3];
    ((void (__fastcall*)(void*, void*, float*))
        CSF_AT(void**, widget, 0)[0x68 / 4])(widget, nullptr, fadedColor);

    C_00830e5c_RenderRadarWidget(self, nullptr, itemBytes);
    return reinterpret_cast<int*>(CHud_AttachPanelsToTarget(
        reinterpret_cast<CHud*>(GetGlobal8afc78()),
        nullptr, reinterpret_cast<int>(widget)));
}

HOOK(0x0043B530, C_00830e5c_DrawMarkerAt, "auto");
