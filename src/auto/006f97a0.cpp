#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ScreenEffects_ReleaseResources, 0x007090F0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, ScreenEffects_DestroyBase, 0x006F8FF0,
         (void* self, void* edx));

static void __fastcall C_00862a80_Destroy(C_00862a80* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862A80);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862A64));
    ScreenEffects_ReleaseResources(self, nullptr);
    ScreenEffects_DestroyBase(self, nullptr);
}

HOOK(0x006F97A0, C_00862a80_Destroy, "auto");
