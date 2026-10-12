#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_008672e4_UpdateVertices(
    C_008672e4* self, void* /*edx*/)
{
    if (!self->field_0010)
        return 0;

    self->field_0010 = 0;

    for (int offset = 0; offset < 56; offset += 28)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c),
               offset + 8) = 0;
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c),
               offset + 12) = 1.0f;

        const uint32_t alpha =
            static_cast<uint8_t>(static_cast<int64_t>(self->field_0030));
        const uint32_t red =
            static_cast<uint8_t>(static_cast<int64_t>(self->field_0024));
        const uint32_t green =
            static_cast<uint8_t>(static_cast<int64_t>(self->field_0028));
        const uint32_t blue =
            static_cast<uint8_t>(static_cast<int64_t>(self->field_002c));

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c),
               offset + 16) =
            (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 0) =
        static_cast<float>(static_cast<int32_t>(self->field_0014));

    const float firstY =
        static_cast<float>(static_cast<int32_t>(self->field_0018));
    const uint32_t returnAddress = self->field_000c;
    CSF_AT(float, reinterpret_cast<void*>(returnAddress), 4) = firstY;

    CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 28) =
        static_cast<float>(static_cast<int32_t>(self->field_001c));
    CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 32) =
        static_cast<float>(static_cast<int32_t>(self->field_0020));

    return static_cast<char>(returnAddress);
}

HOOK(0x00737F40, C_008672e4_UpdateVertices, "auto");
