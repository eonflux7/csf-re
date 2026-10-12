#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static uint32_t PackHalfAlphaVertexColor(float alpha, float red, float green, float blue)
{
    const uint8_t alphaByte = static_cast<uint8_t>(static_cast<int64_t>(alpha));
    const uint8_t redByte = static_cast<uint8_t>(static_cast<int64_t>(red));
    const uint8_t greenByte = static_cast<uint8_t>(static_cast<int64_t>(green));
    const uint8_t blueByte = static_cast<uint8_t>(static_cast<int64_t>(blue));

    return (static_cast<uint32_t>(alphaByte) << 24) |
           (static_cast<uint32_t>(redByte) << 16) |
           (static_cast<uint32_t>(greenByte) << 8) |
           static_cast<uint32_t>(blueByte);
}

static int __fastcall C_008672e4_UpdateVertexColorsWithHalfAlpha(
    C_008672e4* self, void* /*edx*/)
{
    if (self->field_0010)
    {
        const float originalHalfAlpha = self->field_0030;
        int originalBits;
        std::memcpy(&originalBits, &originalHalfAlpha, sizeof(originalBits));

        self->field_0030 = originalHalfAlpha * g_HalfDurationScale;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[2])(self, nullptr);
        self->field_0030 = originalHalfAlpha;
        return originalBits;
    }

    for (int offset = 0; offset < 112; offset += 28)
    {
        uint8_t* vertices = reinterpret_cast<uint8_t*>(self->field_000c);
        CSF_AT(uint32_t, vertices + offset, 0x10) = PackHalfAlphaVertexColor(
            self->field_0030 * g_HalfDurationScale,
            self->field_0024,
            self->field_0028,
            self->field_002c);
    }

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[2])(self, nullptr);

    int result = 0;
    for (int offset = 0; offset < 112; offset += 28)
    {
        result = static_cast<int>(self->field_000c);
        uint8_t* vertices = reinterpret_cast<uint8_t*>(self->field_000c);
        CSF_AT(uint32_t, vertices + offset, 0x10) = PackHalfAlphaVertexColor(
            self->field_0030,
            self->field_0024,
            self->field_0028,
            self->field_002c);
    }

    return result;
}

HOOK(0x00737E50, C_008672e4_UpdateVertexColorsWithHalfAlpha, "auto");
