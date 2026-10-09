#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Geometry_IntersectRaySphere, 0x006ACDF0,
         (void* self, void* edx, float* a2, float* a3, float* a4));

static char __fastcall CGuiControl_HasListenerIntersectionBefore(
    CGuiControl* self, void* /*edx*/, int rayAddress, float maximumDistance)
{
    const uint32_t begin = self->m_listenersBegin;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->m_listenersEnd - begin) / 20)
        : 0;

    char found = 0;
    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 20)
    {
        if (found)
            break;

        float* sphere = reinterpret_cast<float*>(
            self->m_listenersBegin + offset + 4);
        float nearDistance[2] = {};
        float farDistance[2] = {};

        if (Geometry_IntersectRaySphere(
                reinterpret_cast<void*>(rayAddress), nullptr,
                sphere, nearDistance, farDistance))
        {
            if (nearDistance[0] < maximumDistance)
                found = 1;
        }
    }
    return found;
}

HOOK(0x00452F20, CGuiControl_HasListenerIntersectionBefore, "auto");
