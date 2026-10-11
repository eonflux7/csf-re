#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_IsPressed, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(int, __fastcall, C_00830578_QueryInputState, 0x0042B2A0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void __fastcall C_00830578_DispatchControlCommands(
    C_00830578* self, void* /*edx*/)
{
    alignas(8) uint8_t commandBytes[0x10];

    if (MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 11))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        CSF_AT(int, commandBytes, 0x00) = 22;
        DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
    }

    if (MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 12))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        CSF_AT(int, commandBytes, 0x00) = 9;
        DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
    }

    if (MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 2)
        || MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 2)
        || MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 3)
        || MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 3)
        || MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 0)
        || MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 0)
        || static_cast<uint8_t>(
            C_00830578_QueryInputState(self, nullptr, 1)) != 0)
    {
        CSF_AT(int, commandBytes, 0x00) = 18;
    }
    else
    {
        CSF_AT(int, commandBytes, 0x00) = 19;
    }

    CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
    CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
    CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
    DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);

    if (MenuControls_IsPressed(
            reinterpret_cast<void*>(self->field_0014), nullptr, 10)
        || MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 10))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        CSF_AT(int, commandBytes, 0x00) = 20;
        DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
    }
    else
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(int, commandBytes, 0x00) = 21;
        DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
    }
}

HOOK(0x0042B6B0, C_00830578_DispatchControlCommands, "auto");
