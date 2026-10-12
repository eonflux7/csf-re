#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00867358_AppendVertex(
    C_00867358* self, void* /*edx*/,
    const uint32_t* position, const float* color,
    const uint32_t* textureCoordinates)
{
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c) = position[0];
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 4) = position[1];
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 8) = 0;
    CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 12) = 1.0f;

    const int32_t alpha = static_cast<int32_t>(color[3]);
    const int32_t red = static_cast<int32_t>(color[0]);
    const int32_t green = static_cast<int32_t>(color[1]);
    const int32_t blue = static_cast<int32_t>(color[2]);
    const uint32_t packedColor =
        (((((static_cast<uint32_t>(alpha) << 8) |
             static_cast<uint32_t>(red)) << 8) |
           static_cast<uint32_t>(green)) << 8) |
        static_cast<uint32_t>(blue);

    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 16) = packedColor;
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 20) = textureCoordinates[0];
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004),
           28 * self->field_000c + 24) = textureCoordinates[1];

    const uint32_t count = self->field_000c + 1;
    self->field_000c = count;
    return static_cast<int>(count);
}

HOOK(0x00738A10, C_00867358_AppendVertex, "auto");
