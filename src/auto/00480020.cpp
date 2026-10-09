#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Float threshold constant of the original (fcomp dword ptr [0x833164]).
CSF_GLOBAL(float, g_ReloadProgressThreshold, 0x833164);

// If the flag at +0x69 is set, or the progress float at +0x6C exceeds the threshold,
// fetch the helper object from virtual slot 0xD8. If that helper's slot 0x2C reports
// true, run the state-change slot 0x1E4 and the follow-up slot 0x1F0 on this FSM.
static void __fastcall CCharWeaponFsm_TryAdvanceWhenReady(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    if (self->field_0069 || self->field_006c > g_ReloadProgressThreshold)
    {
        int helperRaw = self->vGetIndexedEntryOrZero();
        void* helper = reinterpret_cast<void*>(static_cast<uintptr_t>(helperRaw));

        bool helperReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, helper, 0)[0x2C / 4])(helper, nullptr);
        if (helperReady)
        {
            self->vNotifyWeaponTargetIfBound();
            self->vDispatchIfFlagged();
        }
    }
}

HOOK(0x00480020, CCharWeaponFsm_TryAdvanceWhenReady, "auto");
