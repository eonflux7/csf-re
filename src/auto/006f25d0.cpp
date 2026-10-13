#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(void, __fastcall, GfxParticleEmitters_ReleaseEntry, 0x006E17B0, (void* self, void* , int entryIndex));
CSF_FUNC(void, __fastcall, ParticleEmitters_ReleaseEntry, 0x006E1860, (void* self, void* , int index));

static void __fastcall C_00862598_ReleaseParticleEntries(C_00862598* self, void* /*edx*/)
{
    if (self->field_0024 != 0xFFFFFFFFu)
    {
        uint32_t entryIndex = self->field_0024;
        void* manager = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue_8BB738()));
        GfxParticleEmitters_ReleaseEntry(manager, nullptr, static_cast<int>(entryIndex));
        self->field_0024 = 0xFFFFFFFFu;
    }

    if (self->field_0028 != 0xFFFFFFFFu)
    {
        uint32_t entryIndex = self->field_0028;
        void* manager = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue_8BB738()));
        ParticleEmitters_ReleaseEntry(manager, nullptr, static_cast<int>(entryIndex));
        self->field_0028 = 0xFFFFFFFFu;
    }

    RpPrtStd_ResetBaseState(self, nullptr);
}

HOOK(0x006F25D0, C_00862598_ReleaseParticleEntries, "auto");
