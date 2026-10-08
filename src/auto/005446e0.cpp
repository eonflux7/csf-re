#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CMenuProfiles_SetPendingValue, 0x0053BAE0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, CMenuProfiles_HandleStateTwo, 0x0053C270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuProfilesPC_OpenProfileNameEdit, 0x00544550, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ShutdownAndReturnResult, 0x005442A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetSelection, 0x005442E0, (void* self, void* edx));

// Forwards the event to the CMenuProfiles base handler, then dispatches on the
// state word at +0x38 (cases 0, 2, 4, 5). Any other value is returned unchanged.
static unsigned int __fastcall CMenuProfilesPC_DispatchStateEvent(void* self, void* edx, int a2)
{
    CMenuProfiles_SetPendingValue(self, nullptr, a2);

    const uint32_t state = CSF_AT(uint32_t, self, 0x38);
    switch (state)
    {
    case 0u:
        return static_cast<unsigned int>(CMenuProfilesPC_OpenProfileNameEdit(self, nullptr));
    case 2u:
        return CMenuProfiles_HandleStateTwo(self, nullptr);
    case 4u:
        return static_cast<unsigned int>(CMenuSaveGame_ShutdownAndReturnResult(self, nullptr));
    case 5u:
        return static_cast<unsigned int>(CMenuSaveGame_ResetSelection(self, nullptr));
    default:
        return state;
    }
}

HOOK(0x005446E0, CMenuProfilesPC_DispatchStateEvent, "auto");
