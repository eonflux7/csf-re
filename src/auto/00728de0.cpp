#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, DrawLine, 0x00713C60,
         (void* context, std::uint32_t* start, std::uint32_t* end));

static int __fastcall C_008657e0_DrawOutline(C_008657e0* self, void* /*edx*/)
{
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x18 / 4])(self, nullptr);

    if (!result || !self->field_004c)
        return result;

    std::uint32_t scratch[2];
    std::uint32_t left, top, right, bottom;

    if (self->field_003c || self->field_0040 ||
        self->field_0044 || self->field_0048)
    {
        left = self->field_003c;
        top = self->field_0040;
        right = self->field_0044;
        bottom = self->field_0048;
    }
    else
    {
        void* object =
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));
        auto* position =
            reinterpret_cast<std::uint32_t*(__fastcall*)(
                void*, void*, std::uint32_t*)>(
                CSF_AT(void**, object, 0)[0x160 / 4])(
                    object, nullptr, scratch);
        left = position[0];
        top = position[1];

        auto* size =
            reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0xa4 / 4])(object, nullptr);
        right = left + size[0];
        bottom = top + size[1];
    }

    const std::uint32_t width = right - left;
    std::uint32_t start[2];
    std::uint32_t end[2];

    end[0] = left + width - 1u;
    scratch[0] = left - self->field_0034;
    start[1] = top - self->field_0038;
    start[0] = scratch[0];
    end[0] += self->field_0034;
    end[1] = top - self->field_0038;

    void* context = reinterpret_cast<void*>(self->field_0030);
    auto* color =
        reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, context, 0)[0x30 / 4])(context, nullptr);

    std::uint32_t originalColor[4];
    originalColor[0] = color[0];
    originalColor[1] = color[1];
    originalColor[2] = color[2];
    originalColor[3] = color[3];

    float alpha;
    std::memcpy(&alpha, &originalColor[3], sizeof(alpha));
    const float scaledAlpha = alpha * self->field_0050;

    std::uint32_t outlineColor[4];
    outlineColor[0] = originalColor[0];
    outlineColor[1] = originalColor[1];
    outlineColor[2] = originalColor[2];
    std::memcpy(&outlineColor[3], &scaledAlpha, sizeof(scaledAlpha));

    context = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, context, 0)[0x2c / 4])(
            context, nullptr, outlineColor);

    DrawLine(reinterpret_cast<void*>(self->field_0030), start, end);

    const std::uint32_t height = bottom - top;
    end[1] = top + height - 1u;
    const std::uint32_t leftOffset = self->field_0034;
    scratch[0] = left - leftOffset;
    const std::uint32_t topOffset = self->field_0038;
    scratch[1] = top - topOffset;
    start[0] = scratch[0];
    start[1] = scratch[1];
    end[0] = left - leftOffset;
    end[1] += topOffset;

    DrawLine(reinterpret_cast<void*>(self->field_0030), start, end);

    start[0] = left + width - 1u;
    end[0] = start[0];
    end[1] = top + height - 1u;
    const std::uint32_t rightOffset = self->field_0034;
    scratch[0] = start[0] + rightOffset;
    const std::uint32_t bottomOffset = self->field_0038;
    scratch[1] = top - bottomOffset;
    start[0] = scratch[0];
    start[1] = scratch[1];
    end[0] += rightOffset;
    end[1] += bottomOffset;

    DrawLine(reinterpret_cast<void*>(self->field_0030), start, end);

    const std::uint32_t finalXOffset = self->field_0034;
    const std::uint32_t finalYOffset = self->field_0038;
    scratch[0] = left - finalXOffset;
    scratch[1] = top + height - 1u + finalYOffset;
    end[0] = left + width - 1u + finalXOffset;
    end[1] = scratch[1];
    start[0] = scratch[0];
    start[1] = scratch[1];

    DrawLine(reinterpret_cast<void*>(self->field_0030), start, end);

    context = reinterpret_cast<void*>(self->field_0030);
    return reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, context, 0)[0x2c / 4])(
            context, nullptr, originalColor);
}

HOOK(0x00728DE0, C_008657e0_DrawOutline, "auto");
