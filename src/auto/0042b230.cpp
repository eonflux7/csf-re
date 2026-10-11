#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetValue8B055C, 0x004F9660, ());
CSF_FUNC(bool, __fastcall, IsCheatFlagSet, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ActivateCheatOnObject, 0x004F98A0, (void* self, void* edx, int a2));

// Applies the cheat flags stored in the global state to the cheat menu's flag word.
// Returns the low byte of the last call's result, as the original does (char return).
static char __fastcall C_00830578_ApplyCheatFlags(C_00830578* self, void* /*edx*/) {
    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
    uint8_t returnByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(globalState));

    if (CSF_AT(uint8_t, globalState, 208) != 0) {
        if (IsCheatFlagSet(reinterpret_cast<void*>(self->field_0014), nullptr, 29)) {
            auto* value = reinterpret_cast<C_0083f1f8*>(static_cast<uintptr_t>(GetValue8B055C()));
            ActivateCheatOnObject(value, nullptr, 2);
        }
        if (IsCheatFlagSet(reinterpret_cast<void*>(self->field_0014), nullptr, 30)) {
            auto* value = reinterpret_cast<C_0083f1f8*>(static_cast<uintptr_t>(GetValue8B055C()));
            ActivateCheatOnObject(value, nullptr, 0);
        }

        bool isFlag31Set = IsCheatFlagSet(reinterpret_cast<void*>(self->field_0014), nullptr, 31);
        returnByte = static_cast<uint8_t>(isFlag31Set);
        if (isFlag31Set) {
            auto* value = reinterpret_cast<C_0083f1f8*>(static_cast<uintptr_t>(GetValue8B055C()));
            uint32_t* activated = ActivateCheatOnObject(value, nullptr, 1);
            returnByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(activated));
        }
    }
    return static_cast<char>(returnByte);
}

HOOK(0x0042B230, C_00830578_ApplyCheatFlags, "auto");
