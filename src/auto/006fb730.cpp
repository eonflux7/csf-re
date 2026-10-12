#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ScreenEffects_SetLayerIntensity, 0x00708DE0,
         (void* self, void* edx, int a2, char a3));
CSF_FUNC(char, __fastcall, ScreenEffects_ApplyLayerColor, 0x00708E40,
         (void* self, void* edx, int a2, int a3, int a4, char a5, int a6, float* a7));
CSF_FUNC(int, __stdcall, ScreenEffects_PrepareLayer, 0x00709360, (int a1));

static char __fastcall C_00862cd4_UpdateScreenEffect(
    C_00862cd4* self, void* /*edx*/)
{
    const int globalAccessor = GetGlobalInt();
    GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalAccessor), nullptr);
    SceneLoader_NoOp();

    ScreenEffects_PrepareLayer(static_cast<int>(self->field_0060));

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0054), 0x80))
    {
        const int secondLayer = static_cast<int>(self->field_0064);
        const int firstLayer = static_cast<int>(self->field_0060);
        alignas(8) uint8_t resetBytes[0x8] = {};
        ScreenEffects_ApplyLayerColor(
            self, nullptr, firstLayer, secondLayer, 0xFFFFFF,
            static_cast<char>(255), 0, reinterpret_cast<float*>(resetBytes));

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0054), 0x80) = 0;
    }

    const char intensity = static_cast<char>(self->vSlot20(0));
    ScreenEffects_SetLayerIntensity(
        self, nullptr, static_cast<int>(self->field_0064), intensity);

    const int color = self->vSlot20(3);
    const char opacity = static_cast<char>(self->vSlot20(1));
    const int firstLayer = static_cast<int>(self->field_0060);
    const int secondLayer = static_cast<int>(self->field_0064);
    alignas(8) uint8_t parametersBytes[0x8] = {};

    return ScreenEffects_ApplyLayerColor(
        self, nullptr, firstLayer, secondLayer, color, opacity, 0,
        reinterpret_cast<float*>(parametersBytes));
}

HOOK(0x006FB730, C_00862cd4_UpdateScreenEffect, "auto");
