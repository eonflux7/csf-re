#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Fills a 2-float scale vector (x, y) into the buffer it is given.
CSF_FUNC(unsigned int, __cdecl, Hud_ComputeScaleVector, 0x0042DE60, (float* a1));

// Widget vtable slots used by this routine (slot = byte offset / 4):
//   slot 2  (+0x08): update/refresh, no arguments
//   slot 9  (+0x24): SetPosition(float[2]*)
//   slot 12 (+0x30): GetAnchorPoint(int index) -> float[2]*
//   slot 14 (+0x38): SetSize(int[2]*)
//   slot 15 (+0x3C): GetSize() -> int[2]*

static void __cdecl Hud_RenderWidgetAtScaledRect(int anchorIndex, int* widget, int* rect)
{
    if (widget == nullptr)
    {
        return;
    }

    // Anchor point for anchorIndex (slot 12); both components are read right away.
    float* anchor = ((float* (__fastcall*)(void*, void*, int))CSF_AT(void**, widget, 0)[12])(widget, nullptr, anchorIndex);
    float originalPos[2];
    originalPos[0] = anchor[0];
    originalPos[1] = anchor[1];

    // Current size (slot 15), copied out before anything else changes.
    int* currentSize = ((int* (__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[15])(widget, nullptr);
    int originalSize[2];
    originalSize[0] = currentSize[0];
    originalSize[1] = currentSize[1];

    // Target position and size: taken from rect {left, top, right, bottom} when given,
    // otherwise the anchor point and the widget's current size.
    float scaledPos[2];
    int scaledSize[2];
    if (rect != nullptr)
    {
        scaledPos[0] = (float)rect[0];
        scaledPos[1] = (float)rect[1];
        scaledSize[0] = rect[2] - rect[0];
        scaledSize[1] = rect[3] - rect[1];
    }
    else
    {
        scaledPos[0] = originalPos[0];
        scaledPos[1] = originalPos[1];
        scaledSize[0] = originalSize[0];
        scaledSize[1] = originalSize[1];
    }

    // Scale the position by the first scale vector.
    float scale[2] = {0.0f, 0.0f};
    Hud_ComputeScaleVector(scale);
    scaledPos[0] = scaledPos[0] * scale[0];
    scale[0] = 0.0f;
    scaledPos[1] = scaledPos[1] * scale[1];
    scale[1] = 0.0f;

    // Scale the size by a second, freshly computed scale vector.
    Hud_ComputeScaleVector(scale);
    scaledSize[0] = (int)((float)scaledSize[0] * scale[0]);
    scaledSize[1] = (int)((float)scaledSize[1] * scale[1]);

    // Apply the scaled geometry, refresh, then restore the original geometry.
    ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, widget, 0)[9])(widget, nullptr, scaledPos);
    ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[14])(widget, nullptr, scaledSize);
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[2])(widget, nullptr);
    ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, widget, 0)[9])(widget, nullptr, originalPos);
    ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[14])(widget, nullptr, originalSize);
}

HOOK(0x0042F4F0, Hud_RenderWidgetAtScaledRect, "auto");
