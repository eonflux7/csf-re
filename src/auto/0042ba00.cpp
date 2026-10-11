#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_IsControlActive, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(bool, __fastcall, MenuControls_IsControlReleased, 0x004285F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void Cheats_DispatchZeroVectorCommand(void* command, int commandId)
{
    CSF_AT(int, command, 0x0) = commandId;
    CSF_AT(float, command, 0x4) = g_ZeroVectorX;
    CSF_AT(float, command, 0x8) = g_ZeroVectorY;
    CSF_AT(float, command, 0xC) = g_ZeroVectorZ;
    DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, command);
}

static char __fastcall C_00830578_ProcessCharacterControls(
    C_00830578* self, void* /*edx*/)
{
    alignas(8) uint8_t commandBytes[0x10];

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 13))
        Cheats_DispatchZeroVectorCommand(commandBytes, 2);

    if (!MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 13)
        && !MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 13))
        Cheats_DispatchZeroVectorCommand(commandBytes, 3);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 16))
        Cheats_DispatchZeroVectorCommand(commandBytes, 5);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 17))
        Cheats_DispatchZeroVectorCommand(commandBytes, 10);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 19)
        || MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 24))
        Cheats_DispatchZeroVectorCommand(commandBytes, 12);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 18)
        || MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 27))
        Cheats_DispatchZeroVectorCommand(commandBytes, 11);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 21))
        Cheats_DispatchZeroVectorCommand(commandBytes, 14);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 20))
        Cheats_DispatchZeroVectorCommand(commandBytes, 13);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 14))
    {
        self->field_0040 = 1;
    }
    else if (MenuControls_IsControlReleased(reinterpret_cast<void*>(self->field_0014), nullptr, 14)
             && self->field_0040)
    {
        self->field_0040 = 0;
        Cheats_DispatchZeroVectorCommand(commandBytes, 16);
    }

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 26))
        Cheats_DispatchZeroVectorCommand(commandBytes, 17);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 15))
        Cheats_DispatchZeroVectorCommand(commandBytes, 6);
    else if (MenuControls_IsControlReleased(reinterpret_cast<void*>(self->field_0014), nullptr, 15))
        Cheats_DispatchZeroVectorCommand(commandBytes, 7);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 22))
        Cheats_DispatchZeroVectorCommand(commandBytes, 23);

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 25))
    {
        auto savedVtable = self->vtbl;
        int state = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0x44 / 4])(self, nullptr);
        int toggledState = static_cast<int>(
            (static_cast<uint32_t>(state) & 0xFFFFFF00u)
            | (static_cast<uint8_t>(state) == 0 ? 1u : 0u));
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            savedVtable[0x58 / 4])(self, nullptr, toggledState);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            self->vtbl[0x5C / 4])(self, nullptr, 0);
    }

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 23)
        && !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        auto savedVtable = self->vtbl;
        int state = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0x48 / 4])(self, nullptr);
        int toggledState = static_cast<int>(
            (static_cast<uint32_t>(state) & 0xFFFFFF00u)
            | (static_cast<uint8_t>(state) == 0 ? 1u : 0u));
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            savedVtable[0x5C / 4])(self, nullptr, toggledState);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            self->vtbl[0x58 / 4])(self, nullptr, 0);
    }

    if (MenuControls_IsControlActive(reinterpret_cast<void*>(self->field_0014), nullptr, 36))
        self->vRunActionChecked();

    char result = MenuControls_IsControlActive(
        reinterpret_cast<void*>(self->field_0014), nullptr, 37);
    if (result)
        return self->vCanEnterMenuState();
    return result;
}

HOOK(0x0042BA00, C_00830578_ProcessCharacterControls, "auto");
