#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_ColorsMatchRgba, 0x0042D680, (float* self, void* , float* other));

static void UpdateVertexColor(
    void* vertex, const float* oldColor, const float* newColor)
{
    if (oldColor[0] == CSF_AT(float, vertex, 0x0C) &&
        oldColor[1] == CSF_AT(float, vertex, 0x10) &&
        oldColor[2] == CSF_AT(float, vertex, 0x14) &&
        oldColor[3] == CSF_AT(float, vertex, 0x18))
    {
        CSF_AT(float, vertex, 0x0C) = newColor[0];
        CSF_AT(float, vertex, 0x10) = newColor[1];
        CSF_AT(float, vertex, 0x14) = newColor[2];
        CSF_AT(float, vertex, 0x18) = newColor[3];
    }
    else
    {
        CSF_AT(float, vertex, 0x18) = newColor[3];
    }
}

static int __fastcall C_008670f0_SetColor(
    C_008670f0* self, void* /*edx*/, float* color)
{
    int result = Hud_ColorsMatchRgba(
        reinterpret_cast<float*>(&self->field_00a8), nullptr, color);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    const float oldColor[4] = {
        reinterpret_cast<float&>(self->field_00a8),
        CSF_AT(float, self, 0xAC),
        CSF_AT(float, self, 0xB0),
        CSF_AT(float, self, 0xB4)
    };

    reinterpret_cast<float&>(self->field_00a8) = color[0];
    CSF_AT(float, self, 0xAC) = color[1];
    CSF_AT(float, self, 0xB0) = color[2];
    const float alpha = color[3];
    result = static_cast<int32_t>(self->field_0080);
    CSF_AT(float, self, 0xB4) = alpha;

    uint32_t processed = 0;
    if (result >= 4)
    {
        uint32_t groups = (static_cast<uint32_t>(result - 4) >> 2) + 1;
        processed = 4 * groups;
        uint32_t byteOffset = 0;
        do
        {
            UpdateVertexColor(
                reinterpret_cast<uint8_t*>(self->field_0078) + byteOffset,
                oldColor, color);
            UpdateVertexColor(
                reinterpret_cast<uint8_t*>(self->field_0078) + byteOffset + 28,
                oldColor, color);
            UpdateVertexColor(
                reinterpret_cast<uint8_t*>(self->field_0078) + byteOffset + 56,
                oldColor, color);
            UpdateVertexColor(
                reinterpret_cast<uint8_t*>(self->field_0078) + byteOffset + 84,
                oldColor, color);
            byteOffset += 112;
        } while (--groups);
    }

    if (processed < static_cast<uint32_t>(result))
    {
        uint32_t byteOffset = 28 * processed;
        uint32_t remaining = static_cast<uint32_t>(result) - processed;
        do
        {
            UpdateVertexColor(
                reinterpret_cast<uint8_t*>(self->field_0078) + byteOffset,
                oldColor, color);
            byteOffset += 28;
            result = static_cast<int32_t>(--remaining);
        } while (remaining);
    }

    return result;
}

HOOK(0x00732F60, C_008670f0_SetColor, "auto");
