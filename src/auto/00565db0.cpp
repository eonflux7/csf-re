#include "hook.h"
#include "globals.h"

// Menu accounts module (CMenuAccounts / CMenuMpGameMode / CMenuMpNetworkType).
// Button handler for the "DMatch" entry: forwards the pressed item id to the
// globally selected menu/game-mode object, then runs the shared nullsub tail.

CSF_FUNC(int, __cdecl, Menu_GetSelectedGlobal, 0x521BB0, ());
CSF_FUNC(void, __fastcall, Menu_Global_HandleItem, 0x5231D0, (void* self, void* edx, int itemId));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());

static void __stdcall CMenuMpGameMode_OnDMatchButton(int itemId)
{
    const int target = Menu_GetSelectedGlobal();
    Menu_Global_HandleItem(reinterpret_cast<void*>(target), nullptr, itemId);
    Nullsub_506AC0();
}

HOOK(0x565DB0, CMenuMpGameMode_OnDMatchButton, "auto");
