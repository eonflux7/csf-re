#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static float* __fastcall CGuiTacticalMap_GetVisibleWorldBounds(
    CGuiTacticalMap* self, void* /*edx*/, float* upperCorner, float* lowerCorner)
{
    auto* mapData = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(GetValue8B0554())) + 0x54;

    const float originX = CSF_AT(float, mapData, 0xD7C);
    const float originZ = CSF_AT(float, mapData, 0xD84);
    const float spanX = std::fabs(CSF_AT(float, mapData, 0xD88) - originX);
    const float spanZ = std::fabs(CSF_AT(float, mapData, 0xD90) - originZ);

    // Native unsigned conversion replaces the signed conversion plus
    // the original g_UnsignedIntegerBias correction.
    const float visibleWidth = static_cast<float>(
        static_cast<double>(static_cast<int32_t>(self->m_mapWidth)) /
        static_cast<double>(self->field_0140) * spanX);
    const float visibleHeight = static_cast<float>(
        static_cast<double>(static_cast<int32_t>(self->m_mapHeight)) /
        static_cast<double>(self->field_0144) * spanZ);

    const float upperX = originX - (spanX - visibleWidth) * g_HalfDurationScale;
    upperCorner[0] = upperX;
    upperCorner[2] = originZ - (spanZ - visibleHeight) * g_HalfDurationScale;
    lowerCorner[0] = upperX - visibleWidth;
    lowerCorner[2] = upperCorner[2] - visibleHeight;
    return upperCorner;
}

HOOK(0x004368C0, CGuiTacticalMap_GetVisibleWorldBounds, "auto");
