#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));

// Releases the film resource handle held at +0x60 (if the +0x08 flag is set),
// then chains to the base screen-effects update.
static char __fastcall ScreenEffects_ReleaseFilmResources(C_00862b58* self, void* /*edx*/)
{
    if (self->field_0008 && self->field_0060)
    {
        void** filmResource = reinterpret_cast<void**>(self->field_0060);
        C_00861988* globalInt = reinterpret_cast<C_00861988*>(GetGlobalInt());
        C_00860db0* rwManager = reinterpret_cast<C_00860db0*>(ResolveRwManager(reinterpret_cast<int>(globalInt), nullptr));
        ReleaseManagedResource(rwManager, nullptr, filmResource);
        self->field_0060 = 0;
    }
    return ScreenEffects_UpdateBase(self, nullptr);
}

HOOK(0x7093C0, ScreenEffects_ReleaseFilmResources, "auto");
