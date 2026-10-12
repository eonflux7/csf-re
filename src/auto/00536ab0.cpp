#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __fastcall, MenuMPCharacterSelect_UpdateEnabledState, 0x00542A70, (uint8_t* self, void* , char a2));

// __thiscall(this = ecx, a2 on stack), ret 4.
// If the global state's flag at +0xD0 is set, forward the selection to the
// MP character-select object stored at this+0x14C. Otherwise store a2 at this+0x273.
static int __fastcall Menu_ForwardOrStoreCharacterSelection(uint8_t* self, void* /*edx*/, char a2)
{
    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0))
    {
        uint8_t* mpCharacterSelect = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14C));
        return MenuMPCharacterSelect_UpdateEnabledState(mpCharacterSelect, nullptr, a2);
    }

    CSF_AT(char, self, 0x273) = a2;
    return reinterpret_cast<int>(globalState);
}

HOOK(0x536AB0, Menu_ForwardOrStoreCharacterSelection, "auto");
