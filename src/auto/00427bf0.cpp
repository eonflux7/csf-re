#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, HudObjectives_LayoutWidget, 0x004277F0, (int a1, int a2));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall S_00427bf0_RefreshLayout(S_00427bf0* self, void* /*edx*/)
{
    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* object = LookupObjectById(globalObject, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    alignas(8) uint8_t sizeBytes[0xC];
    int* sizePair = reinterpret_cast<int*>(sizeBytes);
    sizePair[0] = 680;
    sizePair[1] = 600;
    sizePair[2] = 0;
    ScaleHudSizePair(sizePair);

    uint32_t S_00427bf0::* const widgets[] = {
        &S_00427bf0::field_000c,
        &S_00427bf0::field_0010,
        &S_00427bf0::field_0014
    };

    for (int i = 0; i < 3; ++i)
    {
        void* widget = reinterpret_cast<void*>(self->*widgets[i]);
        reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, widget, 0)[0x58 / 4])(widget, nullptr, sizePair);
    }

    HudObjectives_LayoutWidget(
        static_cast<int>(self->field_000c), static_cast<int>(self->field_0024));
    HudObjectives_LayoutWidget(
        static_cast<int>(self->field_0010), static_cast<int>(self->field_0028));
    HudObjectives_LayoutWidget(
        static_cast<int>(self->field_0014), static_cast<int>(self->field_002c));

    alignas(8) uint8_t colorBytes[0x10];
    float* color = reinterpret_cast<float*>(colorBytes);
    int result = 0;

    for (int i = 0; i < 3; ++i)
    {
        void* widget = reinterpret_cast<void*>(self->*widgets[i]);
        const float* currentColor =
            reinterpret_cast<const float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, widget, 0)[0x64 / 4])(widget, nullptr);

        color[0] = currentColor[0];
        color[1] = currentColor[1];
        color[2] = currentColor[2];
        color[3] = 255.0f;

        widget = reinterpret_cast<void*>(self->*widgets[i]);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, color);
    }

    return result;
}

HOOK(0x00427BF0, S_00427bf0_RefreshLayout, "auto");
