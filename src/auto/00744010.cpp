#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void*));
CSF_FUNC(char, __cdecl, NetCallbacks_IsPlayerExcluded, 0x00743E60, (int a1));
CSF_FUNC(void*, __fastcall, NetSpawnMessages_UpdatePlayerSelection, 0x00751C40, (void* self, void* , void* context));
CSF_FUNC(bool, __fastcall, NetSpawnMessages_ContainsValue, 0x00751EC0, (void* self, void* , int value));

CSF_GLOBAL(void*, g_PlayerMatchMessages, 0x008BC140);
CSF_GLOBAL(void*, g_PlayerSelectionMessages, 0x008BC150);
CSF_GLOBAL(void*, g_SelectedPlayerName, 0x008BC154);

static uint32_t* __cdecl NetCallbacks_ProcessPlayerSelection(void* context)
{
    g_SelectedPlayerName = nullptr;
    if (g_PlayerSelectionMessages)
        NetSpawnMessages_UpdatePlayerSelection(g_PlayerSelectionMessages, nullptr, context);

    uint32_t* result = CSF_AT(uint32_t*, context, 0x4);
    if (!result)
        return result;

    result = CSF_AT(uint32_t*, result, 0x4);
    void* node = CSF_AT(void*, result, 0x0);
    if (node == result)
        return result;

    for (;;)
    {
        void* player = CSF_AT(void*, node, 0x8);
        bool matches = false;

        if (GetValueIfNotFlagged(static_cast<uint8_t*>(player) + 0x6B0, nullptr) &&
            !NetCallbacks_IsPlayerExcluded(reinterpret_cast<int>(player)))
        {
            int value = CSF_AT(int, player, 0x2C);
            if ((value & 0x3FFFFF) == 0)
            {
                Game_RaiseStatusNotice(0, 0, -536870898, 0);
                value = 0;
            }

            if (g_PlayerMatchMessages)
                matches = NetSpawnMessages_ContainsValue(g_PlayerMatchMessages, nullptr, value);
        }

        if (matches)
        {
            void* name = MenuMpAccounts_GetPlayerName(player, nullptr);
            g_SelectedPlayerName = name;
            return static_cast<uint32_t*>(name);
        }

        result = CSF_AT(uint32_t*, context, 0x4);
        node = CSF_AT(void*, node, 0x0);
        if (node == CSF_AT(void*, result, 0x4))
            return result;
    }
}

HOOK(0x00744010, NetCallbacks_ProcessPlayerSelection, "auto");
