#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ScreenEffects_ReleaseFilmResources, 0x007093C0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, ScreenEffects_DestroyBase, 0x006F8FF0,
         (void* self, void* edx));

static void __fastcall C_00862b58_Destroy(C_00862b58* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862B58);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862B3C));
    ScreenEffects_ReleaseFilmResources(self, nullptr);
    ScreenEffects_DestroyBase(self, nullptr);
}

HOOK(0x006F9D20, C_00862b58_Destroy, "auto");
