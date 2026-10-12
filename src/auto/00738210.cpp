#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall C_00867318_RefreshVertexColors(
    C_00867318* self, void* /*edx*/)
{
    if (self->field_0010)
    {
        void** vtable = CSF_AT(void**, self, 0);
        const float originalAlpha = self->field_0030;
        self->field_0030 = originalAlpha * g_HalfDurationScale;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(vtable[2])(
            self, nullptr);
        self->field_0030 = originalAlpha;

        int originalBits;
        std::memcpy(&originalBits, &originalAlpha, sizeof(originalBits));
        return originalBits;
    }

    for (int offset = 0; offset < 140; offset += 28)
    {
        const uint8_t alpha = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0030 * g_HalfDurationScale));
        const uint8_t red = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0024));
        const uint8_t green = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0028));
        const uint8_t blue = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_002c));

        uint8_t* vertices = reinterpret_cast<uint8_t*>(self->field_000c);
        CSF_AT(uint32_t, vertices + offset, 0x10) =
            (static_cast<uint32_t>(alpha) << 24) |
            (static_cast<uint32_t>(red) << 16) |
            (static_cast<uint32_t>(green) << 8) |
            static_cast<uint32_t>(blue);
    }

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[2])(self, nullptr);

    // The original keeps the field_000c value it loaded in the last
    // iteration (eax) and returns it, even if the store changes that field.
    uint32_t lastVertexBase = 0;
    for (int offset = 0; offset < 140; offset += 28)
    {
        const uint8_t alpha = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0030));
        const uint8_t red = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0024));
        const uint8_t green = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_0028));
        const uint8_t blue = static_cast<uint8_t>(
            static_cast<int64_t>(self->field_002c));

        lastVertexBase = self->field_000c;
        uint8_t* vertices = reinterpret_cast<uint8_t*>(lastVertexBase);
        CSF_AT(uint32_t, vertices + offset, 0x10) =
            (static_cast<uint32_t>(alpha) << 24) |
            (static_cast<uint32_t>(red) << 16) |
            (static_cast<uint32_t>(green) << 8) |
            static_cast<uint32_t>(blue);
    }

    return static_cast<int>(lastVertexBase);
}

HOOK(0x00738210, C_00867318_RefreshVertexColors, "auto");
