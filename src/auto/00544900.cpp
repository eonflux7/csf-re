#include <cstdint>

#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x00541650 - __cdecl helper; receives the address of the list container embedded in the
// menu object at +0x20. Its single stack argument is cleaned by the caller.
CSF_FUNC(int*, __cdecl, MenuSaveGame_ClearListContainer, 0x541650, (int* pList));

// 0x00541570 - rebuilds/repopulates the list container. The original receives a context
// value in EBX plus two stack arguments (the container at +0x20 and field_001c); both
// stack slots are caller-cleaned ("add esp, 0xC" at 0x00544917 covers them together with
// the argument of 0x541650). The EBX argument cannot be expressed with the C++ calling
// conventions available in this file, so only the two stack slots are forwarded.
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x541570, (int pList, int value));

// CMenuSaveGame::RefreshSaveGameList (0x00544900)
// Empties the save-game list container located at CMenuSaveGame+0x20 and then repopulates
// it using CMenuSaveGame::field_001c (re-read after the reset, because the reset may
// have modified it).
static void __fastcall CMenuSaveGame_RefreshSaveGameList(CMenuSaveGame* self, void* /*edx*/)
{
    int* pList = reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x20);

    MenuSaveGame_ClearListContainer(pList);
    MenuMpStats_ForwardToLayout541230(reinterpret_cast<int>(pList),
                                       static_cast<int>(self->field_001c));
}

HOOK(0x544900, CMenuSaveGame_RefreshSaveGameList, "auto");
