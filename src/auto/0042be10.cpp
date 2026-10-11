#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_QuerySecondaryFlag, 0x00428590, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryHeldFlag, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(double, __fastcall, MenuControls_GetFlagDuration, 0x00428630, (void* self, void* edx, int a2));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __cdecl, Cheats_UpdateControlState, 0x0055BC30, (char a1, float a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_00830578_ProcessControls(
    C_00830578* self, void* /*edx*/, float deltaTime)
{
    alignas(8) uint8_t commandBytes[0x10];

    const uint32_t sceneContext =
        GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneContext)
    {
        if (MenuControls_QueryFlag(
                reinterpret_cast<void*>(self->field_0014), nullptr, 9))
        {
            if ((CSF_AT(uint32_t, reinterpret_cast<void*>(sceneContext), 0x2E0)
                 & 0x1000) == 0)
            {
                CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
                CSF_AT(int, commandBytes, 0x00) = 24;
                CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
                CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
                DispatchSelectedCharacterCommand(
                    g_pCurrentScene, nullptr, commandBytes);
            }
        }
        else if (MenuControls_QuerySecondaryFlag(
                     reinterpret_cast<void*>(self->field_0014), nullptr, 9)
                 && (CSF_AT(uint32_t, reinterpret_cast<void*>(sceneContext), 0x2E0)
                     & 0x1000) != 0)
        {
            CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
            CSF_AT(int, commandBytes, 0x00) = 25;
            CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
            CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
            DispatchSelectedCharacterCommand(
                g_pCurrentScene, nullptr, commandBytes);
        }
    }

    if (MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 14))
    {
        const int settings = GetGlobalValue_8BC1E0();
        const double duration = MenuControls_GetFlagDuration(
            reinterpret_cast<void*>(self->field_0014), nullptr, 14);
        if (duration > CSF_AT(float, reinterpret_cast<void*>(settings), 0x1848))
        {
            if (self->field_0040)
            {
                self->field_0040 = 0;
                CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
                CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
                CSF_AT(int, commandBytes, 0x00) = 15;
                CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
                DispatchSelectedCharacterCommand(
                    g_pCurrentScene, nullptr, commandBytes);
            }
        }
    }

    if (MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 13))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(int, commandBytes, 0x00) = 4;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        DispatchSelectedCharacterCommand(
            g_pCurrentScene, nullptr, commandBytes);
    }

    if (!MenuControls_QueryHeldFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 13)
        && !MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 13))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(int, commandBytes, 0x00) = 3;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        DispatchSelectedCharacterCommand(
            g_pCurrentScene, nullptr, commandBytes);
    }

    if (MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 8))
    {
        const int32_t state = GetGlobal8afc78();
        CSF_AT(uint32_t, reinterpret_cast<void*>(state), 0x270) = 1132396544;
    }

    const int result = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
    {
        const char flag = MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 28);
        Cheats_UpdateControlState(flag, deltaTime);
        return reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0x68 / 4])(self, nullptr);
    }
    return result;
}

HOOK(0x0042BE10, C_00830578_ProcessControls, "auto");
