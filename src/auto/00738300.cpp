#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_00867318_UpdateRectangleVertices(
    C_00867318* self, void* /*edx*/)
{
    if (!self->field_0010)
        return 0;

    self->field_0010 = 0;
    for (int offset = 0; offset < 140; offset += 28)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), offset + 8) = 0;
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), offset + 12) = 1.0f;

        const uint32_t alpha = static_cast<uint8_t>(static_cast<int64_t>(self->field_0030));
        const uint32_t red = static_cast<uint8_t>(static_cast<int64_t>(self->field_0024));
        const uint32_t green = static_cast<uint8_t>(static_cast<int64_t>(self->field_0028));
        const uint32_t blue = static_cast<uint8_t>(static_cast<int64_t>(self->field_002c));
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), offset + 16) =
            (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    void* owner = reinterpret_cast<void*>(self->field_0004);
    const double adjustment =
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x20 / 4])(owner, nullptr);

    const double left = static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment;
    uint32_t returnPointer;

    if (self->field_0034)
    {
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 0) =
            static_cast<float>(left);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 4) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 28) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 32) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0020)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 56) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_001c)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 60) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) - adjustment);

        returnPointer = self->field_000c;
        CSF_AT(float, reinterpret_cast<void*>(returnPointer), 84) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_001c)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 88) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0020)) - adjustment);
    }
    else
    {
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 0) =
            static_cast<float>(left);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 4) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 28) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_001c)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 32) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 56) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_001c)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 60) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0020)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 84) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);

        returnPointer = self->field_000c;
        CSF_AT(float, reinterpret_cast<void*>(returnPointer), 88) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) +
                               static_cast<double>(static_cast<int32_t>(self->field_0020)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 112) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0014)) - adjustment);
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 116) =
            static_cast<float>(static_cast<double>(static_cast<int32_t>(self->field_0018)) - adjustment);
    }

    return static_cast<char>(returnPointer);
}

HOOK(0x00738300, C_00867318_UpdateRectangleVertices, "auto");
