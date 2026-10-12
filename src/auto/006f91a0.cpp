#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

CSF_FUNC(int, __fastcall, GfxScreenEffects_DrawQuad, 0x00708AB0,
         (void* self, void* edx, int a2, char a3, int a4, int a5,
          int a6, uint8_t a7, float a8, float a9, float a10, float a11));
CSF_FUNC(int, __fastcall, GfxScreenEffects_BeginDrawing, 0x00708D10,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_EndDrawing, 0x00708D90,
         (void* self, void* edx));

static int __fastcall C_00862a80_RenderFade(C_00862a80* self, void* /*edx*/)
{
    const int duration = self->vSlot20(0);
    const float progress = static_cast<float>(self->vSlot20(5)) * g_fInv255;
    const int direction = self->vSlot20(1);

    const uint32_t integralProgress =
        static_cast<uint32_t>(static_cast<int64_t>(progress));
    const uint32_t fadeValue = direction == 1
        ? static_cast<uint32_t>(duration) - integralProgress
        : integralProgress;
    const int result = static_cast<int32_t>(255u * fadeValue) / duration;
    const char opacity = static_cast<char>(result);

    if (opacity == 0)
        return result;

    GfxScreenEffects_BeginDrawing(self, nullptr);

    const uint8_t useCustomSize = self->field_0060;
    self->field_0034 =
        static_cast<float>(static_cast<int32_t>(self->field_0064)) -
        g_HalfDurationScale;
    self->field_0038 =
        static_cast<float>(static_cast<int32_t>(self->field_0068)) -
        g_HalfDurationScale;

    const int height = static_cast<int32_t>(
        useCustomSize ? self->field_0070 : self->field_002c);
    const int width = static_cast<int32_t>(
        useCustomSize ? self->field_006c : self->field_0028);
    const int color = self->vSlot20(3);

    GfxScreenEffects_DrawQuad(self, nullptr, color, opacity, width, height,
                             0, 1u, 0.0f, 0.0f, 1.0f, 1.0f);

    self->field_0038 = -0.5f;
    self->field_0034 = -0.5f;
    return GfxScreenEffects_EndDrawing(self, nullptr);
}

HOOK(0x006F91A0, C_00862a80_RenderFade, "auto");
