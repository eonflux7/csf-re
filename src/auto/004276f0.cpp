#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static int __fastcall S_004276c0_UpdateFadeOut(
    S_004276c0* self, void* /*edx*/, float deltaTime)
{
    if (self->field_0044 >= self->field_0040)
    {
        self->field_0044 = 0.0f;
        self->field_0000 = static_cast<uint32_t>(-1);

        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2A2) = 1;
        const int32_t result = GetGlobal8afc78();
        CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x2A1) = 1;
        self->field_0009 = 0;
        return result;
    }

    const float progress = self->field_0044 / self->field_0040;
    const float opacity = g_TransitionOne - progress;

    void* fadeObject = CSF_AT(void*, self, 0x1C);
    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, fadeObject, 0)[0x80 / 4])(
            fadeObject, nullptr, opacity);

    alignas(8) uint8_t colorBytes[0x18];
    CSF_AT(int32_t, colorBytes, 0x00) = CSF_AT(int32_t, self, 0x48);
    CSF_AT(int32_t, colorBytes, 0x04) = CSF_AT(int32_t, self, 0x4C);
    CSF_AT(int32_t, colorBytes, 0x08) = CSF_AT(int32_t, self, 0x50);
    CSF_AT(float, colorBytes, 0x0C) = opacity * CSF_AT(float, self, 0x54);

    void* background = CSF_AT(void*, self, 0x18);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, background, 0)[0x2C / 4])(
            background, nullptr, colorBytes);

    const float textAlpha = g_ColorChannelScale - progress * g_ColorChannelScale;
    uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 0x0C;
    int result = 0;

    for (int remaining = 3; remaining != 0; --remaining, entry += 4)
    {
        void* text = CSF_AT(void*, entry, 0);
        const int32_t* color =
            reinterpret_cast<const int32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, text, 0)[0x64 / 4])(text, nullptr);

        CSF_AT(int32_t, colorBytes, 0x00) = color[0];
        CSF_AT(int32_t, colorBytes, 0x04) = color[1];
        CSF_AT(int32_t, colorBytes, 0x08) = color[2];
        CSF_AT(float, colorBytes, 0x0C) = textAlpha;

        text = CSF_AT(void*, entry, 0);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, text, 0)[0x60 / 4])(text, nullptr, colorBytes);
    }

    self->field_0044 = deltaTime + self->field_0044;
    return result;
}

HOOK(0x004276F0, S_004276c0_UpdateFadeOut, "auto");
