#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CommitOverlayBuffer, 0x006EF050, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00862640_UpdateOverlay, 0x006F2F00, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static int __fastcall C_00862640_RenderOverlay(C_00862640* self, void* /*edx*/)
{
    void* overlay = reinterpret_cast<void*>(self->field_0020);
    ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, overlay, 0)[0x34 / 4])(overlay, nullptr, 1);

    if (self->field_0024)
        C_00862640_UpdateOverlay(self, nullptr);

    void* opacitySource = reinterpret_cast<void*>(self->field_0048);
    double opacity;
    bool shouldDraw = true;

    if (opacitySource)
    {
        opacity = ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, opacitySource, 0)[0x8 / 4])(opacitySource, nullptr);
        opacity = opacity * self->field_0038 * self->field_0034;
        shouldDraw = opacity > g_fZero;
    }
    else
    {
        opacity = self->field_0038 * self->field_0034;
    }

    if (shouldDraw)
    {
        uint32_t color = CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_0018), 0x54);
        const int64_t alpha = static_cast<int64_t>(opacity * g_ColorChannelScale);
        color = (color & 0x00FFFFFFu)
              | (static_cast<uint32_t>(static_cast<uint8_t>(alpha)) << 24);

        overlay = reinterpret_cast<void*>(self->field_0020);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, overlay, 0)[0x30 / 4])(
                overlay, nullptr, static_cast<int>(color));

        CommitOverlayBuffer(reinterpret_cast<void*>(self->field_0020), nullptr);
    }

    overlay = reinterpret_cast<void*>(self->field_0020);
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, overlay, 0)[0x34 / 4])(overlay, nullptr, 0);
}

HOOK(0x006F3270, C_00862640_RenderOverlay, "auto");
