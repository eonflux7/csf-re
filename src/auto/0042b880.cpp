#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_IsControlActive, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(int, __fastcall, C_00830578_IsActionActive, 0x0042B2A0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_00830578_UpdateCharacterControlFlags(
    C_00830578* self, void* /*edx*/, int /*unused*/)
{
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    int state = ResolveEntryLookupValue(provider, nullptr);
    if (state == 4)
        return state;

    globalValue = GetGlobalValue_4FBDE0();
    firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    state = ResolveEntryLookupValue(provider, nullptr);
    if (state == 5)
        return state;

    alignas(8) uint8_t commandBytes[0x10];

    const bool primaryActive =
        MenuControls_IsControlActive(
            reinterpret_cast<void*>(self->field_0014), nullptr, 2) ||
        MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 2) ||
        static_cast<uint8_t>(C_00830578_IsActionActive(self, nullptr, 3)) ||
        static_cast<uint8_t>(C_00830578_IsActionActive(self, nullptr, 0)) ||
        static_cast<uint8_t>(C_00830578_IsActionActive(self, nullptr, 1));

    CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
    CSF_AT(int, commandBytes, 0x00) = primaryActive ? 18 : 19;
    CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
    CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
    DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);

    if (MenuControls_IsControlActive(
            reinterpret_cast<void*>(self->field_0014), nullptr, 10) ||
        MenuControls_QueryFlag(
            reinterpret_cast<void*>(self->field_0014), nullptr, 10))
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
        CSF_AT(int, commandBytes, 0x00) = 20;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
    }
    else
    {
        CSF_AT(float, commandBytes, 0x04) = g_ZeroVectorX;
        CSF_AT(float, commandBytes, 0x08) = g_ZeroVectorY;
        CSF_AT(int, commandBytes, 0x00) = 21;
        CSF_AT(float, commandBytes, 0x0C) = g_ZeroVectorZ;
    }

    DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
    return 0;
}

HOOK(0x0042B880, C_00830578_UpdateCharacterControlFlags, "auto");
