#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall CGuiControl_ScalePositionAndSize(
    CGuiControl* self, void* /*edx*/, float scaleX, float scaleY)
{
    const float scaledX = static_cast<std::int32_t>(self->m_x) * scaleX;
    const float scaledY = static_cast<std::int32_t>(self->m_y) * scaleY;
    const float scaledWidth = static_cast<std::int32_t>(self->m_width) * scaleX;
    const float scaledHeight = static_cast<std::int32_t>(self->m_height) * scaleY;

    const int roundedY = static_cast<int>(static_cast<std::int64_t>(
        scaledY < g_fZero
            ? scaledY - g_HalfDurationScale
            : scaledY + g_HalfDurationScale));
    const int roundedX = static_cast<int>(static_cast<std::int64_t>(
        scaledX < g_fZero
            ? scaledX - g_HalfDurationScale
            : scaledX + g_HalfDurationScale));

    int dimensions[2] = {roundedX, roundedY};
    self->vSetPosition(dimensions);

    const int roundedHeight = static_cast<int>(static_cast<std::int64_t>(
        scaledHeight < g_fZero
            ? scaledHeight - g_HalfDurationScale
            : scaledHeight + g_HalfDurationScale));
    const int roundedWidth = static_cast<int>(static_cast<std::int64_t>(
        scaledWidth < g_fZero
            ? scaledWidth - g_HalfDurationScale
            : scaledWidth + g_HalfDurationScale));

    dimensions[0] = roundedWidth;
    dimensions[1] = roundedHeight;
    self->vSetSize(dimensions);

    void* effects = reinterpret_cast<void*>(self->m_pEffects);
    if (effects) {
        return reinterpret_cast<int (__fastcall*)(void*, void*, float, float)>(
            CSF_AT(void**, effects, 0)[0x78 / 4])(
                effects, nullptr, scaleX, scaleY);
    }
    return 0;
}

HOOK(0x007198A0, CGuiControl_ScalePositionAndSize, "auto");
