#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);
CSF_GLOBAL(void*, g_Block8BBF8C, 0x008BBF8C);

static void __fastcall C_00867444_AppendGlyphQuads(
    C_00867444* self, void* /*edx*/, float* parameters)
{
    if (self->field_0050 >= 0x64)
        return;

    const float baseline = static_cast<float>(
        self->vGetScaledValue(1) * g_CharacterHeightScale);

    float left;
    // Only left/baseline are spilled to single precision here. Top and extents
    // stay on the x87 stack until the individual vertex stores.
    double top;
    double width;
    double height;

    if (self->field_005e)
    {
        void* metricObject = reinterpret_cast<void*>(self->field_0008);
        const float positionedX = static_cast<float>(static_cast<int32_t>(
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x20) +
            static_cast<uint32_t>(static_cast<int64_t>(parameters[1]))));

        const double horizontalMetric =
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, metricObject, 0)[0x20 / 4])(
                    metricObject, nullptr);
        left = static_cast<float>(positionedX - horizontalMetric);

        metricObject = reinterpret_cast<void*>(self->field_0008);
        const float positionedY = static_cast<float>(static_cast<int32_t>(
            static_cast<uint32_t>(
                static_cast<int64_t>(static_cast<double>(baseline) + parameters[2])) -
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x24)));

        const double verticalMetric =
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, metricObject, 0)[0x20 / 4])(
                    metricObject, nullptr);
        top = positionedY - verticalMetric;

        void* glyph = CSF_AT(void*, parameters, 0);
        width = static_cast<double>(CSF_AT(int32_t, glyph, 0x18));
        height = static_cast<double>(CSF_AT(int32_t, glyph, 0x1C));
    }
    else
    {
        void* glyph = CSF_AT(void*, parameters, 0);
        left = static_cast<double>(CSF_AT(int32_t, glyph, 0x20)) *
                   self->field_0054 + parameters[1];
        top = static_cast<double>(baseline) + parameters[2] -
              static_cast<double>(CSF_AT(int32_t, glyph, 0x24)) *
                  self->field_0058;
        width = static_cast<double>(CSF_AT(int32_t, glyph, 0x18)) *
                self->field_0054;
        height = static_cast<double>(CSF_AT(int32_t, glyph, 0x1C)) *
                 self->field_0058;
    }

    const uint32_t red =
        static_cast<uint32_t>(static_cast<int64_t>(parameters[3])) & 0xFF;
    const uint32_t green =
        static_cast<uint32_t>(static_cast<int64_t>(parameters[4])) & 0xFF;
    const uint32_t blue =
        static_cast<uint32_t>(static_cast<int64_t>(parameters[5])) & 0xFF;
    const uint32_t alpha =
        static_cast<uint32_t>(static_cast<int64_t>(parameters[6])) & 0xFF;

    const uint32_t glyphColor =
        blue | (green << 8) | (red << 16) | (alpha << 24);
    const bool shadow = self->field_005c != 0;
    const uint32_t firstColor = shadow ? (alpha << 24) : glyphColor;
    const int layerCount = (shadow != 0) + 1;
    uint32_t offset = 28 * (4 * layerCount * self->field_0050);

    for (int layer = 0; layer < layerCount; ++layer, offset += 112)
    {
        float x = left;
        double y = top;
        if (self->field_005c && layer == 0)
        {
            x = left + self->field_0054;
            y = top + self->field_0058;
        }

        CSF_AT(float, g_Block8BBF8C, offset) = x;
        CSF_AT(float, g_Block8BBF8C, offset + 4) = y;
        const float bottom = y + height;
        CSF_AT(float, g_Block8BBF8C, offset + 28) = x;
        CSF_AT(float, g_Block8BBF8C, offset + 32) = bottom;
        const float right = x + width;
        CSF_AT(float, g_Block8BBF8C, offset + 56) = right;
        CSF_AT(float, g_Block8BBF8C, offset + 60) = y;
        CSF_AT(float, g_Block8BBF8C, offset + 84) = right;
        CSF_AT(float, g_Block8BBF8C, offset + 88) = bottom;

        const uint32_t color = layer == 0 ? firstColor : glyphColor;
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 16) = color;
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 44) = color;
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 72) = color;
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 100) = color;

        CSF_AT(uint32_t, g_Block8BBF8C, offset + 20) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x08);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 24) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x0C);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 48) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x08);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 52) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x14);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 76) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x10);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 80) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x0C);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 104) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x10);
        CSF_AT(uint32_t, g_Block8BBF8C, offset + 108) =
            CSF_AT(uint32_t, CSF_AT(void*, parameters, 0), 0x14);
    }

    ++self->field_0050;
}

HOOK(0x00739620, C_00867444_AppendGlyphQuads, "auto");
