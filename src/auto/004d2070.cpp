#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, MpSpawn_GetSelectedObjectFlagByte, 0x00551490, (void* self, void*));
CSF_FUNC(bool, __fastcall, SpawnPoints_IsSabotageSpawnReady, 0x00551530, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpStats_GetPlayerExclusionFlag, 0x007411A0, (CMenuMpStats* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_RemoveFromSceneCategory, 0x0048E6B0, (C_00833908* self, void*));
CSF_FUNC(void, __fastcall, S_004d1d90_ResetState, 0x004D1D90, (void* self, void* edx));

// The original never stores a vtable pointer here; this binding only keeps the
// S_004d1d90 vtable address referenced by this function (not a behavioral store).
[[maybe_unused]] static const auto s_S004d1d90Vtable = CSF_VTBL(0x004D1D90);

static void __fastcall S_004d1d90_UpdateSabotageSpawnState(S_004d1d90* self, void* /*edx*/, CMenuMpStats* a2)
{
    if (self->field_0060
        && !static_cast<uint8_t>(CMenuMpStats_GetPlayerExclusionFlag(a2, nullptr))
        && MpSpawn_GetSelectedObjectFlagByte(
               reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8B0688())), nullptr) == 2
        && CSF_AT(uint8_t, a2, 0x443))
    {
        self->field_009c = 0x3f000000u;
    }
    else
    {
        int globalValue = GetGlobal8B0688();
        if ((SpawnPoints_IsSabotageSpawnReady(
                 reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr)
             || !CSF_AT(uint8_t, a2, 0x443))
            && !self->field_0061)
        {
            C_00833908_RemoveFromSceneCategory(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr);
            self->field_0061 = 1;
            S_004d1d90_ResetState(self, nullptr);
            self->field_0060 = 1;
        }
    }
}

HOOK(0x004D2070, S_004d1d90_UpdateSabotageSpawnState, "auto");
