#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets the PTank atomic-create parameter at +0xC0 to a2 and ORs flag 0x20000 into +0x40
// of the PTank object that this object's slot refers to through g_PTankPluginOffset.
// Returns the PTank object pointer.
static int __fastcall C_00862110_SetPTankParamAndFlag(C_00862110* self, void* /*edx*/, int a2) {
    CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x8B09EC);

    uint8_t* ptank = *reinterpret_cast<uint8_t**>(
        static_cast<uintptr_t>(g_PTankPluginOffset) + self->field_0004);
    CSF_AT(int, ptank, 0xC0) = a2;

    uint8_t* ptankAgain = *reinterpret_cast<uint8_t**>(
        static_cast<uintptr_t>(g_PTankPluginOffset) + self->field_0004);
    CSF_AT(uint32_t, ptankAgain, 0x40) |= 0x20000u;

    return reinterpret_cast<int>(ptankAgain);
}

HOOK(0x6EEB10, C_00862110_SetPTankParamAndFlag, "auto");
