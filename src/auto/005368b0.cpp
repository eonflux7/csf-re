// Reimplementation of 0x005368B0 (interface, INSTANCING SCENE/RESULT).
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

CSF_FUNC(char, __fastcall, S_0042cc30_ApplyFlagThenRefresh, 0x0042CC30, (S_0042cc30* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_536650, 0x00536650, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_CommitCharacterSelection, 0x00542820, ());

// S_0042cc30's vtable reference (namespace-scope object, so it is a declaration, not a statement).
[[maybe_unused]] static void** const g_S_0042cc30_Vtable = CSF_VTBL(0x0042CC30);

static char __fastcall S_0042cc30_CommitSelectionIfFlagged(S_0042cc30* self, void* /*edx*/, int a2)
{
    if (!self->field_013d) {
        return S_0042cc30_ApplyFlagThenRefresh(self, nullptr, a2);
    }

    char result = CSF_AT(char, reinterpret_cast<char*>(a2), 0x1c);
    if (result) {
        void* subObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x134));
        if (subObject) {
            // vtable slot 168 / 4 = 42 : (obj, 1)
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, subObject, 0)[42])(subObject, nullptr, 1);
            // vtable slot 88 / 4 = 22 : (obj)
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, subObject, 0)[22])(subObject, nullptr);
        }

        self->field_013d = 0;

        void* outer = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) - 4);
        sub_536650(outer, nullptr, 1);

        // The original loads [this+0x148] into ecx before this call; the packet bind is __cdecl ()
        // so the value is read here for order fidelity but not passed.
        uint32_t selectionOwner = CSF_AT(uint32_t, self, 0x148);
        CSF_AT(int, self, 0x130) = 2;
        MenuMpCharacterSelect_CommitCharacterSelection();
        (void)selectionOwner;

        int globalState = GetGlobalState_50D190();
        MenuSystem_ToggleChildFlag(reinterpret_cast<void*>(globalState), nullptr);

        // vtable slot 184 / 4 = 46 : (outer)
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, outer, 0)[46])(outer, nullptr));
    }
    return result;
}

HOOK(0x005368B0, S_0042cc30_CommitSelectionIfFlagged, "auto");
