#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binding for the sub-routine at 0x536650 (name chosen locally; signature from packet).
CSF_FUNC(int, __fastcall, MenuScreen_ApplyStateReset, 0x00536650, (void* self, void* edx, char a2));

CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_CommitCharacterSelection, 0x00542820, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void* edx));

static hook::Tail __fastcall MenuMpCharacterSelect_ResetToCommitState(void* self, const uint32_t* /*args*/)
{
    C_00830578* screen = reinterpret_cast<C_00830578*>(self);

    // Child object stored at +0x138: if present, call its vtable slot 0xA8 (arg 1), then slot 0x58.
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, screen, 0x138)));
    if (child) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr, 1);

        child = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, screen, 0x138)));
        reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr);
    }

    CSF_AT(uint8_t, screen, 0x141) = 0;
    MenuScreen_ApplyStateReset(screen, nullptr, 0);

    CSF_AT(int, screen, 0x134) = 2;
    MenuMpCharacterSelect_CommitCharacterSelection();

    int globalState = GetGlobalState_50D190();
    MenuSystem_ToggleChildFlag(reinterpret_cast<void*>(static_cast<intptr_t>(globalState)), nullptr);

    // Tail jump through this->vtbl[0xB0 / 4] with `this` in ecx.
    void* obj = screen;
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xB0 / 4]);
}

HOOK_TAIL(0x00536CD0, MenuMpCharacterSelect_ResetToCommitState, "interface");
