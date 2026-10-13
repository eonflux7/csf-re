#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, CGuiTacticalMap_GetWorldBounds, 0x004368C0,
         (void* self, void* edx, float* a2, float* a3));

static uint32_t* __fastcall CGuiTacticalMap_WorldToMap(
    CGuiTacticalMap* self, void* /*edx*/, uint32_t* output, float* position)
{
    if (!self->field_0158) {
        output[0] = 0;
        output[1] = 0;
        return output;
    }

    alignas(8) uint8_t firstCornerBytes[0x14]{};
    alignas(8) uint8_t secondCornerBytes[0x14]{};
    float* firstCorner = reinterpret_cast<float*>(firstCornerBytes);
    float* secondCorner = reinterpret_cast<float*>(secondCornerBytes);

    CGuiTacticalMap_GetWorldBounds(self, nullptr, firstCorner, secondCorner);

    const uint32_t mapY = self->m_mapY;
    const uint32_t mapX = self->m_mapX;
    const uint32_t mapWidth = self->m_mapWidth;
    const uint32_t mapHeight = self->m_mapHeight;

    const int32_t worldWidth = static_cast<int32_t>(
        static_cast<int64_t>(std::fabs(secondCorner[0] - firstCorner[0])));
    const int32_t worldHeight = static_cast<int32_t>(
        static_cast<int64_t>(std::fabs(secondCorner[2] - firstCorner[2])));

    const uint32_t distanceX = static_cast<uint32_t>(
        static_cast<int64_t>(std::fabs(position[0] - secondCorner[0])));
    const int32_t offsetX =
        static_cast<int32_t>(mapWidth * distanceX) / worldWidth;

    const uint32_t distanceZ = static_cast<uint32_t>(
        static_cast<int64_t>(std::fabs(position[2] - secondCorner[2])));
    const int32_t offsetY =
        static_cast<int32_t>(mapHeight * distanceZ) / worldHeight;

    output[0] = mapWidth - static_cast<uint32_t>(offsetX) + mapX;
    output[1] = mapHeight - static_cast<uint32_t>(offsetY) + mapY;
    return output;
}

HOOK(0x00436D30, CGuiTacticalMap_WorldToMap, "auto");
