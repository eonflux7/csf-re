#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fMaxSceneStep, 0x00861180);

CSF_FUNC(int, __fastcall, S_006d6800_ProcessSceneUpdate, 0x006D6800, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, C_00863044_ProcessSceneEntries, 0x006FF390, (C_00863044* self, void* , int a3));
CSF_FUNC(char, __fastcall, S_006ff7e0_RefreshPlayerEntries, 0x00700DA0, (S_006ff7e0* self, void*));
CSF_FUNC(int, __cdecl, RwImageSetHeight, 0x007FB000, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwImageSetWidth, 0x007FB010, (int a1, int a2));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));

[[maybe_unused]] static const auto kPacketSceneUpdateVtable = CSF_VTBL(0x006D6800);

static char __fastcall S_006d6800_UpdateScene(
    S_006d6800* self, void* /*edx*/, float deltaTime)
{
    if (deltaTime < g_fZero)
        deltaTime = 0.0f;
    else if (deltaTime > g_fMaxSceneStep)
        deltaTime = 0.039999999f;

    if (self->field_003c && self->field_00d8 && deltaTime > g_fZero)
    {
        uint8_t changed = 0;
        for (uint32_t index = 0; index < self->field_005c; ++index)
        {
            void* entry = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0054), 4 * index);

            if (((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, entry, 0)[0x28 / 4])(entry, nullptr))
            {
                changed |= static_cast<uint8_t>(
                    ((int(__fastcall*)(void*, void*, float))
                        CSF_AT(void**, entry, 0)[0x70 / 4])(
                            entry, nullptr, deltaTime));
            }
        }

        int deltaBits;
        std::memcpy(&deltaBits, &deltaTime, sizeof(deltaBits));
        C_00863044_ProcessSceneEntries(
            reinterpret_cast<C_00863044*>(
                reinterpret_cast<uint8_t*>(self) + 0x74),
            nullptr, deltaBits);

        int width;
        if (changed)
        {
            int image = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
            RwImageSetHeight(image, 600);
            width = 400;
        }
        else
        {
            int image = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
            RwImageSetHeight(image, 300);
            width = 200;
        }

        int image = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
        RwImageSetWidth(image, width);
        S_006d6800_ProcessSceneUpdate(self, nullptr, deltaTime);
    }

    char refresh = static_cast<char>(self->field_00da);
    if (refresh)
    {
        for (uint32_t index = 0; index < self->field_005c; ++index)
        {
            S_006ff7e0_RefreshPlayerEntries(
                CSF_AT(S_006ff7e0*,
                    reinterpret_cast<void*>(self->field_0054), 4 * index),
                nullptr);
        }

        return static_cast<char>(
            ((int(__fastcall*)(S_006d6800*, void*))
                reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(
                    self, nullptr));
    }

    return refresh;
}

HOOK(0x006D6CD0, S_006d6800_UpdateScene, "auto");
