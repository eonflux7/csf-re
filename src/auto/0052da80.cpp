#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game CRT sprintf (cdecl). The original pushes exactly one int vararg per call.
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, int value));

// __thiscall with one stack argument (ret 4), bound as __fastcall.
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, void* block));

// Debug heap free (cdecl). Game heap Mem_Free is intentionally not used.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Looks up a child control by name through the owner's vtable slot 63 (+0xFC).
// The owner pointer is re-read from the screen on every lookup, as the original does.
static void* ScenarioSelect_FindControlByName(CMenuScenarioSelect* self, const char* controlName)
{
    void* owner = reinterpret_cast<void*>(self->field_0018);
    using FindControlFn = void* (__fastcall*)(void*, void*, const char*);
    return reinterpret_cast<FindControlFn>(CSF_AT(void**, owner, 0)[63])(owner, nullptr, controlName);
}

// Calls vtable slot 42 (+0xA8) on a control with argument 1.
static void ScenarioSelect_SetControlFlagOne(void* control)
{
    using SetFlagFn = void(__fastcall*)(void*, void*, int);
    reinterpret_cast<SetFlagFn>(CSF_AT(void**, control, 0)[42])(control, nullptr, 1);
}

static void __fastcall CMenuScenarioSelect_ClearBriefingAndResetStars(CMenuScenarioSelect* self, void* /*edx*/)
{
    const uint32_t childObjectAddress = self->field_0068;
    if (childObjectAddress == 0) {
        return;
    }

    // Unwind/cleanup state. Kept volatile so the cleanup branch (and its
    // Mem_FreeDebug call) is preserved exactly as in the original.
    volatile uint32_t unwindState = 7;

    // 20-byte string-like block: low word of block[0] is zeroed (empty string), block[4] is null.
    void* block[5];
    block[4] = nullptr;
    *reinterpret_cast<uint16_t*>(&block[0]) = 0;
    C_00720DF0_ForwardCallToChildVtblSlot16(reinterpret_cast<void*>(childObjectAddress), nullptr, block);

    for (uint32_t index = 1; index <= 5; ++index) {
        char nameN[56];
        char name[52];
        _sprintf(nameN, CSF_STR(0x008415FC, "EstrellaN%d"), static_cast<int>(index));
        _sprintf(name, CSF_STR(0x008415F0, "Estrella%d"), static_cast<int>(index));

        void* controlN = ScenarioSelect_FindControlByName(self, nameN);
        if (controlN != nullptr) {
            ScenarioSelect_SetControlFlagOne(controlN);
        }

        void* control = ScenarioSelect_FindControlByName(self, name);
        if (control != nullptr) {
            ScenarioSelect_SetControlFlagOne(control);
        }
    }

    if (unwindState >= 8) {
        if (block[0] != nullptr) {
            Mem_FreeDebug(block[0]);
        }
    }
}

HOOK(0x0052DA80, CMenuScenarioSelect_ClearBriefingAndResetStars, "auto");
