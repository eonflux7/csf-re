#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ScreenEffects_ReleaseResources, 0x006FBB30,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, ScreenEffects_DestroyBase, 0x006F8FF0,
         (void* self, void* edx));

static void __fastcall C_00862d48_Destroy(C_00862d48* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862D48);
    CSF_AT(uint32_t, self, 0x04) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862D2C));
    ScreenEffects_ReleaseResources(self, nullptr);
    ScreenEffects_DestroyBase(self, nullptr);
}

HOOK(0x006FC040, C_00862d48_Destroy, "auto");
