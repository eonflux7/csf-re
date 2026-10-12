#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float* __fastcall C_008630e0_GetAveragePlayerPosition(
    C_008630e0* self, void* /*edx*/, float* result)
{
    if (static_cast<std::uint8_t>(self->vSlot34()) == 0)
    {
        result[0] = g_ZeroVectorX;
        result[1] = g_ZeroVectorY;
        result[2] = g_ZeroVectorZ;
        return result;
    }

    void* team = reinterpret_cast<void*>(self->field_0004);
    const unsigned int count = CSF_AT(std::uint16_t, team, 0x0C);
    const float weight = g_TransitionOne / static_cast<float>(count);

    float x = g_fZero;
    float y = g_fZero;
    float z = g_fZero;

    if (count != 0)
    {
        auto* players = CSF_AT(std::uint8_t*, team, 0x10);
        for (unsigned int i = 0; i < count; ++i)
        {
            void* player = CSF_AT(void*, players + 12 * i, 0x04);
            if (player)
            {
                void* object = CSF_AT(void*, player, 0x00);
                void* positionData = CSF_AT(void*, object, 0x10);
                auto* positions = CSF_AT(std::uint8_t*, positionData, 0x08);
                const std::uint32_t index = CSF_AT(std::uint32_t, player, 0x04);
                const auto* position = positions + 32 * index;

                x += weight * CSF_AT(float, position, 0x00);
                y += weight * CSF_AT(float, position, 0x04);
                z += weight * CSF_AT(float, position, 0x08);
            }
        }
    }

    result[0] = x;
    result[1] = y;
    result[2] = z;
    return result;
}

HOOK(0x006FFDE0, C_008630e0_GetAveragePlayerPosition, "auto");
